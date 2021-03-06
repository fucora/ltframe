/*
    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "../testwindow.h"
#include "../util.h"

#include <QScopedPointer>
#include <QtTest/QtTest>
#include <qquickwebpage_p.h>
#include <qquickwebview_p.h>

class tst_QQuickWebView : public QObject {
    Q_OBJECT
public:
    tst_QQuickWebView();

private slots:
    void init();
    void cleanup();

    void accessPage();
    void navigationStatusAtStartup();
    void stopEnabledAfterLoadStarted();
    void baseUrl();
    void loadEmptyUrl();
    void loadEmptyPageViewVisible();
    void loadEmptyPageViewHidden();
    void loadNonexistentFileUrl();
    void backAndForward();
    void reload();
    void stop();
    void loadProgress();
    void scrollRequest();

    void show();
    void showWebView();
    void removeFromCanvas();
    void multipleWebViewWindows();
    void multipleWebViews();

private:
    inline QQuickWebView* webView() const;
    QScopedPointer<TestWindow> m_window;
};

tst_QQuickWebView::tst_QQuickWebView()
{
    addQtWebProcessToPath();
    qRegisterMetaType<QQuickWebPage*>("QQuickWebPage*");
}

void tst_QQuickWebView::init()
{
    m_window.reset(new TestWindow(new QQuickWebView()));
}

void tst_QQuickWebView::cleanup()
{
    m_window.reset();
}

inline QQuickWebView* tst_QQuickWebView::webView() const
{
    return static_cast<QQuickWebView*>(m_window->webView.data());
}

void tst_QQuickWebView::accessPage()
{
    QQuickWebPage* const pageDirectAccess = webView()->page();

    QVariant pagePropertyValue = webView()->experimental()->property("page");
    QQuickWebPage* const pagePropertyAccess = pagePropertyValue.value<QQuickWebPage*>();
    QCOMPARE(pagePropertyAccess, pageDirectAccess);
}

void tst_QQuickWebView::navigationStatusAtStartup()
{
    QCOMPARE(webView()->canGoBack(), false);

    QCOMPARE(webView()->canGoForward(), false);

    QCOMPARE(webView()->loading(), false);

    QCOMPARE(webView()->canReload(), false);
}

class LoadStartedCatcher : public QObject {
    Q_OBJECT
public:
    LoadStartedCatcher(QQuickWebView* webView)
        : m_webView(webView)
    {
        connect(m_webView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    }

public slots:
    void onLoadStarted()
    {
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);

        QCOMPARE(m_webView->loading(), true);
    }

signals:
    void finished();

private:
    QQuickWebView* m_webView;
};

void tst_QQuickWebView::stopEnabledAfterLoadStarted()
{
    QCOMPARE(webView()->loading(), false);

    LoadStartedCatcher catcher(webView());
    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    waitForSignal(&catcher, SIGNAL(finished()));

    QCOMPARE(webView()->loading(), true);

    waitForSignal(webView(), SIGNAL(loadSucceeded()));
}

void tst_QQuickWebView::baseUrl()
{
    // Test the url is in a well defined state when instanciating the view, but before loading anything.
    QVERIFY(webView()->url().isEmpty());
}

void tst_QQuickWebView::loadEmptyUrl()
{
    webView()->load(QUrl());
    webView()->load(QUrl(QLatin1String("")));
}

void tst_QQuickWebView::loadEmptyPageViewVisible()
{
    m_window->show();
    loadEmptyPageViewHidden();
}

void tst_QQuickWebView::loadEmptyPageViewHidden()
{
    QSignalSpy loadStartedSpy(webView(), SIGNAL(loadStarted()));

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(loadStartedSpy.size(), 1);
}

void tst_QQuickWebView::loadNonexistentFileUrl()
{
    QSignalSpy loadFailedSpy(webView(), SIGNAL(loadStarted()));

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/file_that_does_not_exist.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadFailed(QQuickWebView::ErrorDomain, int, QUrl, QString))));

    QCOMPARE(loadFailedSpy.size(), 1);
}

void tst_QQuickWebView::backAndForward()
{
    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page2.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page2.html"));

    webView()->goBack();
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    webView()->goForward();
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page2.html"));
}

void tst_QQuickWebView::reload()
{
    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    webView()->reload();
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));
}

void tst_QQuickWebView::stop()
{
    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QCOMPARE(webView()->url().path(), QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html"));

    // FIXME: This test should be fleshed out. Right now it's just here to make sure we don't crash.
    webView()->stop();
}

void tst_QQuickWebView::loadProgress()
{
    QCOMPARE(webView()->loadProgress(), 0);

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QSignalSpy loadProgressChangedSpy(webView(), SIGNAL(loadProgressChanged(int)));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    QVERIFY(loadProgressChangedSpy.count() >= 1);

    QCOMPARE(webView()->loadProgress(), 100);
}

void tst_QQuickWebView::show()
{
    // This should not crash.
    m_window->show();
    QTest::qWait(200);
    m_window->hide();
}

void tst_QQuickWebView::showWebView()
{
    webView()->setSize(QSizeF(300, 400));

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/direct-image-compositing.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    m_window->show();
    // This should not crash.
    webView()->setVisible(true);
    QTest::qWait(200);
    webView()->setVisible(false);
    QTest::qWait(200);
}

void tst_QQuickWebView::removeFromCanvas()
{
    showWebView();

    // This should not crash.
    QQuickItem* parent = webView()->parentItem();
    QQuickItem noCanvasItem;
    webView()->setParentItem(&noCanvasItem);
    QTest::qWait(200);
    webView()->setParentItem(parent);
    webView()->setVisible(true);
    QTest::qWait(200);
}

void tst_QQuickWebView::multipleWebViewWindows()
{
    showWebView();

    // This should not crash.
    QQuickWebView* webView1 = new QQuickWebView();
    QScopedPointer<TestWindow> window1(new TestWindow(webView1));
    QQuickWebView* webView2 = new QQuickWebView();
    QScopedPointer<TestWindow> window2(new TestWindow(webView2));

    webView1->setSize(QSizeF(300, 400));
    webView1->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForSignal(webView1, SIGNAL(loadSucceeded())));
    window1->show();
    webView1->setVisible(true);

    webView2->setSize(QSizeF(300, 400));
    webView2->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView2, SIGNAL(loadSucceeded())));
    window2->show();
    webView2->setVisible(true);
    QTest::qWait(200);
}

void tst_QQuickWebView::multipleWebViews()
{
    showWebView();

    // This should not crash.
    QScopedPointer<QQuickWebView> webView1(new QQuickWebView());
    webView1->setParentItem(m_window->rootItem());
    QScopedPointer<QQuickWebView> webView2(new QQuickWebView());
    webView2->setParentItem(m_window->rootItem());

    webView1->setSize(QSizeF(300, 400));
    webView1->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForSignal(webView1.data(), SIGNAL(loadSucceeded())));
    webView1->setVisible(true);

    webView2->setSize(QSizeF(300, 400));
    webView2->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/basic_page.html")));
    QVERIFY(waitForSignal(webView2.data(), SIGNAL(loadSucceeded())));
    webView2->setVisible(true);
    QTest::qWait(200);
}

void tst_QQuickWebView::scrollRequest()
{
    webView()->setSize(QSizeF(300, 400));

    webView()->load(QUrl::fromLocalFile(QLatin1String(TESTS_SOURCE_DIR "/html/scroll.html")));
    QVERIFY(waitForSignal(webView(), SIGNAL(loadSucceeded())));

    // COMPARE with the position requested in the html
    // Use qRound as that is also used when calculating the position
    // in WebKit.
    int y = -qRound(50 * webView()->page()->contentScale());
    QVERIFY(webView()->page()->pos().y() == y);
}

QTWEBKIT_API_TEST_MAIN(tst_QQuickWebView)

#include "tst_qquickwebview.moc"


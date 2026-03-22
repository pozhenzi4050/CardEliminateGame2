#include "AppDelegate.h"
#include"Views/GameScene.h"
#include <windows.h>  // 新增头文件

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}


// 解决LNK2001的核心：必须实现的OpenGL初始化方法
void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    SetConsoleOutputCP(CP_UTF8);


    if (!glview) {
        glview = GLViewImpl::createWithRect("CardEliminate", Rect(0, 0, 1080, 2080), 0.5f);
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    auto scene = GameScene::createScene();
    director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
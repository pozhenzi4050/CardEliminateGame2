#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#include "cocos2d.h"

class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    // Cocos2d-x强制要求实现：OpenGL上下文初始化
    virtual void initGLContextAttrs() override;
    // 应用启动完成
    virtual bool applicationDidFinishLaunching() override;
    // 应用进入后台
    virtual void applicationDidEnterBackground() override;
    // 应用回到前台
    virtual void applicationWillEnterForeground() override;
};

#endif // APP_DELEGATE_H
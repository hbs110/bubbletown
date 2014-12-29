/*!
 * \file TexturePool.h
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePool 
*/

#pragma once

class TexturePool 
{
public: 
    TexturePool() {}
    ~TexturePool() {}

    /*
        初始化纹理池（可以通过下面的参数来控制内存的占用情况）
            internalTextureCount - 内部维护贴图的数量
            internalTextureSize - 内部维护贴图的尺寸 
    */
    bool Init(int internalTextureCount, int internalTextureSize);
    
    // 销毁纹理池，释放所有占用的内存
    void Destroy();


    /*
        把精灵放入池中
            tag - 如果指定这个参数，相同 tag 的贴图将会被放入同一张内部贴图
            manualReleaseRequired - 如果为 true 则必须手动释放，如果为 false 则会在纹理池已满或手动整理时被释放。

        实现细节：
            该 sprite 指向的原贴图将被复制入纹理池，操作成功后 sprite 将指向池内的对应位置
     */
    bool PushSprite(cocos2d::Sprite* sprite, const std::string& tag = "", bool manualReleaseRequired = false);
    
    // 从纹理池中移除该精灵，注意，该精灵将会恢复对原始贴图的引用，如果成功的话仍可用于后续绘制
    void ReleaseSprite(cocos2d::Sprite* sprite);

    /*
        整理纹理的使用情况
            releaseIfPossible - 如果为 true 则会释放那些没有强制要求手动释放的精灵，如果为 false 则仅仅只是重新排列在纹理中的位置
     */
    void Defrag(bool releaseIfPossible = true);

private:

};


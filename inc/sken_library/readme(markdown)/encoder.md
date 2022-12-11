<!-- リファレンスのテンプレート -->
<!-- readmeにライブラリのリファレンスを記載 -->

# encoder

インクリメンタル式ロータリーエンコーダのパルスをカウントするクラス．

# class Encoder

## void Encoder::init(Pin a_pin,Pin b_pin,TimerNumber tim_num)
エンコーダのピンとタイマー番号を設定する．  
使用することができるタイマーは1,2,3,4,5,8のチャンネル1とチャンネル2である．  
エンコーダのカウントに使用するタイマーはチャンネル3とチャンネル4も含めてPWM出力に使用することはできない．

[パラメータ]  
チャンネル1のピン番号  
チャンネル2のピン番号  
タイマー番号

[戻り値]  
なし

[サンプルコード]
タイマー2をエンコーダモードに設定する

``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Encoder encoder;

int main(void)
{
    sken_system.init();
    encoder.init(A0,A1,TIMER2);
    while(1)
    {

    }
}
```

## int Encoder::read(void)
カウンタの値を取得することができる．  
カウンタの値は-30000から30000までである．

[パラメータ]  
なし

[戻り値]  
カウンタ値

[サンプルコード]


``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Encoder encoder;

int count;

int main(void)
{
    sken_system.init();
    encoder.init(A0,A1,TIMER2);
    while(1)
    {
        count = encoder.read();
    }
}
```

## void Encoder::reset(void)
カウンタの値をリセットする

[パラメータ]  
なし

[戻り値]  
なし

[サンプルコード]
カウンタの値が10000を超えた時にリセットする

``` c++
#include "stm32f4xx.h"
#include "sken_library/include.h"

Encoder encoder;

int count;

int main(void)
{
    sken_system.init();
    encoder.init(A0,A1,TIMER2);
    while(1)
    {
        count = encoder.read();
        if(count > 10000){
            encoder.reset();
        }
    }
}
```
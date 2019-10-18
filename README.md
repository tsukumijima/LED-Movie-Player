
# LED-Movie-Player

動画・音声を読み込んで LED マトリクスパネル上で再生させる Raspberry Pi 用アプリケーション  
（ https://qiita.com/cstoku/items/eb17d111225d3e35ec61 の記事上のコードより改造）です。

必要なものや配線等は [こちら](https://blog.tsukumijima.net/article/ledmatrix-movie/) の記事も参考にしてください。

## 同梱物

 - led-movie-player … Raspberry Pi 3 B+ 上でビルドした実行ファイル
   - 基本的に Linux では実行ファイルに拡張子が付きません
 - led-movie-player.cc … ソース（ C++ ）ファイル
   - 表示・再生部分のコードを前述の記事よりお借りしています
 - led-movie-convert … YouTube 等の動画やラズパイ上の動画を変換して LED-Movie-Player で再生出来る形式にするソフト
   - 中身はバッチファイルです（後述）
 - Makefile … ビルド手順等を記述したスクリプトファイル
 - README.md … このファイル
 - sample/ … LEDで流せるサイズにエンコードしたサンプルの mp4 と mp3 を入れてあります
   - 再生させる場合は一式 /home/pi/movieplayer/ にコピーして下さい
 - matrix/ … [rpi-rgb-lex-matrix](https://github.com/hzeller/rpi-rgb-led-matrix) を submodule として使っています

## ビルド手順

一応ビルド済みのファイルも同梱していますが、出来るだけビルドすることを推奨します

1. 予め必要なパッケージ（ライブラリ）をインストールしておきましょう
    - `sudo apt update -y && sudo apt upgrade -y`
    - `sudo apt install -y python3 python3-pip ffmpeg gcc g++ make libopencv-dev libsdl1.2-dev libsdl2-dev libsdl-mixer1.2-dev libsdl2-mixer-dev`
    - `sudo pip3 install youtube-dl`
2. make でビルドします
    - `make`
    - ビルドに失敗する場合、ライブラリがうまくインストール出来ていない可能性があります
3. make install でパスの通った場所にインストールします
    - `sudo make install`
    - ちゃんと sudo をつけましょう
4. その他、デフォルトではラズパイの bcm2835 サウンドカードを無効化する必要があります（ハードウェアパルスの生成に必要なため）
    - そのため、別途 USB サウンドカードを購入し、USB サウンドカードで再生できる（ラズパイ内蔵サウンドカードを使わない）状態にする必要があります
    - ハードウェアパルスを生成せずソフトウェアパルスで再生する事も可能ですが、ちらつきが発生するため非推奨です
      - ハードウェアパルスを無効化して再生させたい場合は、led-movie-player.cc 中の options.disable_hardware_pulsing = false; を options.disable_hardware_pulsing = true; に書き換え、もう一度ビルドしてください
    - 詳細な手順は [こちら](https://blog.tsukumijima.net/article/ledmatrix-movie/#toc11) の項目に記載してあります、参考にしてください

## 使い方

1. 動画は最大 192 × 108 までにリサイズした .mp4 形式、音声はサンプリングレート 44100kHz にした .mp3 形式にし、同じファイル名で /home/pi/movieplayer/ に保存します。
   - LED-Movie-Convert を使うと、動画・音声の変換を自動化できます
   - /home/pi/movieplayer/ 以外に置くことも可能ですが、その場合は led-movie-player.cc ソース内の記述を変更してください
2. sudo led-movie-player "[ 拡張子なしファイル名 ]" と入力して実行すると、再生できます。
   - 拡張子なしファイル名は、予め /home/pi/movieplayer/ に置いてあるファイルのものにしてください
     - 例えば /home/pi/movieplayer/ に TEIDA ZONE.mp4 と TEIDA ZONE.mp3 を置いた場合、[ 拡張子なしファイル名 ] には "TEIDA ZONE" を入れます
     - "TEIDA ZONE" のようにファイル名にスペースが含まれる場合は、必ず "" で囲うようにしてください
   - sudo をつけないとエラーになります、必ず sudo をつけて root で実行してください
   - 最後のコマンド以外はデフォルトで 64 × 64 パネルを 2 個直列に接続している事を想定して再生します。
     - 解像度が異なるパネルの場合は、led-movie-player.cc ソース内の記述（ 155 行目あたり）を変更してください
   - sudo led-movie-player "[ 拡張子なしファイル名 ]" 0 と入力して実行すると、無限ループで再生します。
   - sudo led-movie-player "[ 拡張子なしファイル名 ]" [ ループ回数 ] と入力して実行すると、引数に記載された回数だけ有限ループで再生します。
   - sudo led-movie-player "[ 拡張子なしファイル名 ]" [ ループ回数 ] [ LED パネルの縦幅 ] [ LED パネルの横幅 ] [ LED パネルの直列接続数 ] [ LED パネルの並列接続数 ]
と入力して実行すると、引数に記載されたパラメータで再生を実行します。
3. Ctrl+C を押すと中断できます。

## 使用例
 - 基本：sudo led-movie-player "TEIDA ZONE"
 - 無限ループ：sudo led-movie-player "TEIDA ZONE" 0
 - 有限ループ：sudo led-movie-player "TEIDA ZONE" 15
 - 引数を全て指定：sudo led-movie-player "TEIDA ZONE" 1 64 64 2 1

 ## 注意
 - 64 × 64 のパネルを 2 個直列につないだ LED パネルを想定しています。
   - 他の解像度のパネルでは動かないかもしれません
 - 動作確認は Raspberry Pi 3 B+ 上の Raspbian で行っています。
   - もしかすると、他のラズパイではそのままのコードでは動かない、という事があるかもしれません
 - コマンドラインから再生します。GUI はありません。
   - ラズパイ自体が非力なため、GUI があるとかえって描画が重くなるそう…（ rpi-rgb-led-matrix 曰く）
 - ろくに検証もしていない自分用ソフト（元々改造品で配布するつもりなかったけど勿体無いので…）です。他の環境で動くかは微妙…
 - ソースに変更を加えたら、当然ですが再ビルドしないと変更は反映されません。
 - たまに Ctrl+C 押しても動画が流れ続ける場合があります（プロセスが裏で動きっぱなしになってしまう）…
   - その場合は、`ps aux | grep "led-movie-player (クオーテーションなし拡張子なしファイル名) (ループ指定した場合は入れる)" | grep -v grep | awk '{ print "sudo kill -9", $2 }' | sh`
     - 例：ps aux | grep "led-movie-player TEIDA ZONE 0" | grep -v grep | awk '{ print "sudo kill -9", $2 }' | sh
   - と実行すると強制終了出来るはずです。

## 動画・音声のエンコード

前述の通り、LED パネルで再生させる動画や音声は、予め LED-Movie-Player で再生できる形式に変換する必要があります。

 - 動画
   - .mp4 形式
   - 最大でも 192 × 108 にリサイズする
     - 720p とかの動画をそのまま突っ込むと重すぎてスロー再生状態になります
     - 160 × 90 の方が音ズレしません
       - 192 × 108 はギリギリです
   - 音声は mp3 から再生するのであってもなくても可
 - 音声
   - .mp3 形式
   - ビットレートを128kbpsにする
     - 192kbps とかでもいけるかもしれませんが一応
   - サンプルレートを必ず44100kHzにする
     - 48000kHz だと音がバグる…

LED-Movie-Convert を利用して、YouTube 等の動画やラズパイ上にある動画を自動で変換させることができます。  
出来るだけこちらを利用する事をお勧めします（後述）

手動でエンコードする場合、ffmpeg で予め変換しておきましょう（ Windows でも動くので、PC でエンコードしてラズパイに移すのもありです）

動画のエンコードコマンド例: ffmpeg -i "Movie.mp4" -vf scale=192:108 "Movie-led.mp4"  
音声のエンコードコマンド例: ffmpeg -i "Movie.mp3" -vn -af dynaudnorm -ac 2 -ar 44100 -ab 128k "Movie-led.mp3"

変換が終わったら、変換後のファイルをラズパイの /home/pi/movieplayer/ 以下にコピーしてください。  
前述の通り、led-movie-player.cc ソース内の記述を変更し、フォルダを変更することも可能です

## LED-Movie-Convert

### 注意

 - バッチファイルです (パスの通った場所に置けばコマンド同様に実行できます)
 - sudo make installする際についででパスの通った場所にインストールされます
 - ラズパイのスペックが低い関係でエンコードに時間がかかります、気長に待ちましょう
 - エンコード中は負荷が非常に高くなるので、エンコード中に LED-Movie-Player で再生させようとするとスロー再生になってしまう事があります
   - (ラズパイの性能が低いため LED パネルへの表示処理が実際の再生スピードに追い付かない)

### 使い方
 - 第一引数… ソース元を指定します ( URL or ファイルパス)
 - 第二引数… ソースの種類を選択します ( URL からダウンロードするなら download 、ファイルパスからなら file )
 - 第三引数… ファイル名を設定します (ファイル名を動画名 or ファイル名から取るなら auto 、それ以外なら拡張子なしのファイル名を入れる)
 - 第四引数… コントラストを上げるかどうか（上げるなら true・上げないなら false ）
   - アニメなど、コントラストの低い動画は上げたほうがより鮮明に、より綺麗に再生できます
 - 第五引数(オプション)… 動画の横幅を指定（例: 160 ）
 - 第六引数(オプション)… 動画の高さを指定（例: 90 ）

### 使用例

 - YouTube 等からダウンロードする場合
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download auto false
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download auto true
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE" false
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE" true
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE" false 192 108
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE" true 192 108
 - ローカルファイルから変換する場合
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file auto false
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file auto true
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file "TEIDA ZONE" false
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file "TEIDA ZONE" true
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file "TEIDA ZONE" false 160 90
   - led-movie-convert "/home/pi/TEIDA ZONE 【飯田駅×TEI ZONE】.mp4" file "TEIDA ZONE" true 160 90

## 謝辞

改造元の記事（ https://qiita.com/cstoku/items/eb17d111225d3e35ec61 ）の著者の cstoku さん、素晴らしいコードを本当にありがとうございます…   
（私だけではまず書けないコードでした・表示処理はほぼそのままです）

## License
[MIT Licence](LICENSE.txt)

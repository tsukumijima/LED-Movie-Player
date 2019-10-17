#include <iostream>
#include <stdio.h>
#include <zconf.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <chrono>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "led-matrix.h"
#include "graphics.h"
#include <sys/types.h>
#include <sys/stat.h>


using namespace std; 
using namespace rgb_matrix;

// LEDマトリクスで動画を再生する関数
// 引数1: 動画のファイル名
// 引数2: 音声のファイル名
// 引数3: 個々のLEDパネルの縦幅
// 引数4: 個々のLEDパネルの横幅
// 引数5: 直列に繋いでいるLEDパネルの数
// 引数6: 並列に繋いでいるLEDパネルの数

int play(std::string movie_file, std::string audio_file, int rows, int cols, int chain, int parallel){

    // LEDパネルのパラメータをOptionsに設定
    RGBMatrix::Options options;
    options.rows = rows;
    options.cols = cols;
    options.chain_length = chain;
    options.parallel = parallel;
    options.disable_hardware_pulsing = true; // ハードウェアパルス生成を無効にする(有効にする場合音声モジュールを無効化する必要がある)

    // GPIOを初期化
    GPIO io;

    if (!io.Init())
        return 1;

    // 音声の読み込み

    SDL_Init(SDL_INIT_AUDIO); // SDLを初期化
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096); // ミキサーAPIを初期化・音声ファイルのサンプルレートと第一引数が異なると音ズレする
    Mix_Music *music = Mix_LoadMUS(audio_file.c_str()); // 音声ファイルをロード
    if (!music)
        return 1;

    // LEDマトリクスの初期化

    RGBMatrix *matrix = new RGBMatrix(&io, options); // RGBMatrixを初期化
    matrix->set_luminance_correct(true);
    matrix->SetBrightness(100);
    matrix->SetPWMBits(11);
    FrameCanvas *canvas = matrix->CreateFrameCanvas(); // 描画Canvasを初期化
    const int width = canvas->width();
    const int height = canvas->height();

    // 動画の読み込み

    cv::Mat src_img;
    cv::Mat dst_img(height, width, src_img.type());
    cv::VideoCapture cap(movie_file.c_str()); // VideoCaptureを初期化

    auto fps = cap.get(CV_CAP_PROP_FPS); // 動画のfpsを取得
    long fps_ustime = 1000000.0 / (double)fps + 0.5;

    const auto hoseicount = (int)fps * 10;
    auto hcnt = 0;

    // 音声を再生する

    if (Mix_PlayMusic(music, 1) == -1) {
        printf("    Mix_PlayMusic: %s\n", Mix_GetError());
        return 1;
    }
    auto pre_dc = 1ll;

    // 動画を再生する

    auto frame_cnt = cap.get(CV_CAP_PROP_FRAME_COUNT) - 1; // Magic!!!
    auto start = std::chrono::system_clock::now();

    // 1フレームごとに処理
    while (true){

        auto fpos = cap.get(CV_CAP_PROP_POS_FRAMES);
        // fposが総フレーム数を超えたら再生を終了する
        if(!(fpos < frame_cnt)){
            std::wcout << "    Stoping Play the Movie." << std::endl;
            break;
        }

        // フレームを取得(Capture)
        cap >> src_img;

        // LEDマトリクスに描画
        auto pos = cap.get(CV_CAP_PROP_POS_MSEC);
        auto r = (double) height / src_img.rows;
        auto w = (int) (src_img.cols * r);
        auto d = (width - w) / 2;
        cv::resize(src_img, dst_img, cv::Size(w, height), cv::INTER_AREA); // LEDマトリクスのサイズに合わせて取得したフレームをリサイズする
        for (int y = 0; y < height; y++){
            for (int x = 0; x < w; x++){
                cv::Vec3b bgr = dst_img.at<cv::Vec3b>(y, x);
                canvas->SetPixel(x + d, y, bgr[2], bgr[1], bgr[0]); // 1ピクセルごとに描画
            }
        }

        // 同期を取る
        matrix->SwapOnVSync(canvas);
        auto end = std::chrono::system_clock::now();

        auto delaytime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        auto delaycount = pos * 1000 - delaytime;
        if (delaycount > 0) {
            usleep(delaycount);
        }
    }

    // 終了処理

    matrix->Clear();
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    delete matrix;

}

// エントリーポイント

int main(int argc, char *argv[]){

    int cols; // 個々のLEDパネルの横幅
    int rows; // 個々のLEDパネルの縦幅
    int chain; // 直列に繋いでいるLEDパネルの数
    int parallel; // 並列に繋いでいるLEDパネルの数
    int loop; // ループ回数
    std::string loopstr; // 諸問題で文字列型のループ回数
    int loopflg; // ループフラグ
    std::string version = "2.3"; // バージョン
    std::string file; // ファイル(拡張子なし)
    struct stat st;

    // 引数確認
    // 変数argvは文字列なのでatoiでint型に変換しておく

    if (argc == 2){ // 引数が3個(コマンド名と動画ファイル名指定のみ)

        // デフォルト値で再生
        file = argv[1];
        loopstr = "none";

        cols = 64; // 個々のLEDパネルの横幅・適宜変更してください
        rows = 64; // 個々のLEDパネルの縦幅・適宜変更してください
        chain = 2; // 直列に繋いでいるLEDパネルの数・適宜変更してください
        parallel = 1; // 並列に繋いでいるLEDパネルの数・適宜変更してください

    } else if (argc == 3){ // 引数が3個(コマンド名と動画ファイル名とループ指定のみ)

        // デフォルト値で再生
        file = argv[1];
        loop = atoi( argv[2] );
        loopstr = argv[2]; //0が入るとNULLと判断されてしまうため敢えて文字型

        cols = 64; // 個々のLEDパネルの横幅・適宜変更してください
        rows = 64; // 個々のLEDパネルの縦幅・適宜変更してください
        chain = 2; // 直列に繋いでいるLEDパネルの数・適宜変更してください
        parallel = 1; // 並列に繋いでいるLEDパネルの数・適宜変更してください

    } else if (argc == 7){ // 引数が6個(LEDパネルの幅・高さ・個数等も指定)

        // 指定サイズで再生
        file = argv[1];
        loop = atoi( argv[2] );
        loopstr = argv[2]; // 0が入るとNULLと判断されてしまうため文字型にする
        cols = atoi( argv[3] );
        rows = atoi( argv[4] );
        chain = atoi( argv[5] );
        parallel = atoi( argv[6] );

    } else { // それ以外はエラー吐いて終了

        std::wcout << "    " << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "  Error: Argument is missing or too many." << std::endl;
        std::wcout << "  Usage: 1: sudo led-movie-player [File name (no extension)]" << std::endl;
        std::wcout << "            ... Play" << std::endl;
        std::wcout << "         2: sudo led-movie-player [File name (no extension)] 0" << std::endl;
        std::wcout << "            ... Play (Infinite loop)" << std::endl;
        std::wcout << "         3: sudo led-movie-player [File name (no extension)] [Number of loops]" << std::endl;
        std::wcout << "            ... Play (Specified number of loops)" << std::endl;
        std::wcout << "         4: sudo led-movie-player [File name (no extension)] [Number of loops]" << std::endl;
        std::wcout << "                 [LED panel vertical width] [LED panel horizontal width]" << std::endl;
        std::wcout << "                 [Chain LED panels] [Parallel LED panels]" << std::endl;
        std::wcout << "            ... Play (Specify all arguments)" << std::endl;
        std::wcout << "    " << std::endl;
        std::wcout << "  Please retry... m(__)m" << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "    " << std::endl;
        return 1;
    }

    int panelwidth = cols * chain ; // LEDパネルの横幅の合計
    int panelheight = rows * parallel ; // LEDパネルの縦幅の合計

    // 検索ディレクトリ・適宜変更してください
    std::string path = "/home/pi/movieplayer/";

    // 再生するファイルを検索して代入
    std::string movie_file = path + file + ".mp4"; // 動画ファイル
    std::string audio_file = path + file + ".mp3"; // 音声ファイル(MP3・M4Aなど他の形式に変えても読み込める)

    // sudoで実行されてなかったら(Rootでないなら)エラー吐いて終了
    if (geteuid() != 0){
        std::wcout << "    " << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "  Error: It must be run with Root privileges." << std::endl;
        std::wcout << "  Please add 'sudo' at the beginning" << std::endl;
        std::wcout << "  of the command and retry... m(__)m." << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "    " << std::endl;
        return 1;
    }

    // ファイルがなかったらエラー吐いて終了
	if (stat(movie_file.c_str(), &st) != 0 || stat(audio_file.c_str(), &st) != 0){
        std::wcout << "    " << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "  Error: Specified file Not found." << std::endl;
        std::wcout << "  Please retry... m(__)m" << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;
        std::wcout << "    " << std::endl;
        return 1;
    }

    // ループ判別
    if (loopstr == "none"){ // ループ再生ではないとき
        loopflg = 0;
    } else if (loop == 0){ // 無限ループ
        loopflg = 1;
    } else if (loop >= 1){ // 有限ループ
        loopflg = 2;
    } else {
        loopflg = 0; //パラメータ変だったら取りあえず一回だけ
    }

    // 表示部分

    std::wcout.imbue(std::locale("ja_JP.utf8"));
    std::wcout << "    " << std::endl;
    std::wcout << "---------------------------------------------------" << std::endl;
    std::wcout << "               LED-Movie-Player v" << version.c_str() << std::endl;
    std::wcout << "---------------------------------------------------" << std::endl;
    std::wcout << "    Chain LED Panels    : " << chain << std::endl;
    std::wcout << "    Parallel LED Panels : " << parallel << std::endl;
    std::wcout << "    One LED Panel Size  : " << cols << "x" << rows << std::endl;
    std::wcout << "    All LED Panel Sizes : " << panelwidth << "x" << panelheight << std::endl;
    std::wcout << "    " << std::endl;
    if (loopflg == 0){
        std::wcout << "    Press Ctrl + C to stop the movie." << std::endl;
    } else if (loopflg == 1 || loopflg == 2){
        std::wcout << "    If you want to stop loop, Press Ctrl + C." << std::endl;
    }
    std::wcout << "---------------------------------------------------" << std::endl;

    if (loopflg == 0){ // ループ再生しない

        // 表示
        std::wcout << "    Start playing movie..." << std::endl;
        std::wcout << "    " << std::endl;
        std::wcout << "    Now playing..." << std::endl;
        std::wcout << "---------------------------------------------------" << std::endl;

        // 再生を実行
        int result = play(movie_file, audio_file, rows, cols, chain, parallel);

        // 終了
        std::wcout << "    See You!!" << std::endl;
    
    } else if (loopflg == 1){ // 無限ループ

        while (1){ // 無限ループ実行

            // 表示
            std::wcout << "    Start playing movie..." << std::endl;
            std::wcout << "    " << std::endl;
            std::wcout << "    Now Playing..." << std::endl;
            std::wcout << "---------------------------------------------------" << std::endl;

            // 再生実行
            int result = play(movie_file, audio_file, rows, cols, chain, parallel);

            // 少し待つ
            std::wcout << "    Now looping..." << std::endl;
            std::wcout << "---------------------------------------------------" << std::endl;
            int second = 300000;
            usleep(second);
        }

    } else if (loopflg == 2){ // 有限ループ

        for (int i = 1; i <= loop; i++){ // ループ実行

            // 表示
            std::wcout << "    Start playing movie..." << std::endl;
            std::wcout << "    Number of loops   : " << loop << std::endl;
            std::wcout << "    Current loop count: "  << i << std::endl;
            std::wcout << "    " << std::endl;
            std::wcout << "    Now Playing..." << std::endl;
            std::wcout << "---------------------------------------------------" << std::endl;

            // 再生実行
            int result = play(movie_file, audio_file, rows, cols, chain, parallel);

            if (i < loop){
                // 少し待つ
                std::wcout << "    Now looping..." << std::endl;
                std::wcout << "---------------------------------------------------" << std::endl;
                int second = 300000;
                usleep(second);
            } else if (i == loop){ // 最後のループなら
                std::wcout << "    See You!!" << std::endl;
            }
            
        }

    }

    std::wcout << "---------------------------------------------------" << std::endl;
    std::wcout << "    " << std::endl;

}


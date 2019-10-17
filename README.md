
# LED-Movie-Player

����E������ǂݍ���� LED �}�g���N�X�p�l����ōĐ������� Raspberry Pi �p�A�v���P�[�V����  
�i https://qiita.com/cstoku/items/eb17d111225d3e35ec61 �̋L����̃R�[�h�������j�ł��B

## ������

 - led-movie-player �c Raspberry Pi 3 B+ ��Ńr���h�������s�t�@�C��
   - ��{�I�� Linux �ł͎��s�t�@�C���Ɋg���q���t���܂���
 - led-movie-player.cc �c �\�[�X�i C++ �j�t�@�C��
   - �\���E�Đ������̃R�[�h��O�q�̋L����肨�؂肵�Ă��܂�
 - led-movie-convert �c YouTube ���̓���⃉�Y�p�C��̓����ϊ����� LED-Movie-Player �ōĐ��o����`���ɂ���\�t�g
   - ���g�̓o�b�`�t�@�C���ł��i��q�j
 - Makefile �c �r���h�菇�����L�q�����X�N���v�g�t�@�C��
 - README.md �c ���̃t�@�C��
 - sample/ �c LED�ŗ�����T�C�Y�ɃG���R�[�h�����T���v���� mp4 �� mp3 �����Ă���܂�
   - �Đ�������ꍇ�͈ꎮ /home/pi/movieplayer/ �ɃR�s�[���ĉ�����
 - matrix/ �c [rpi-rgb-lex-matrix](https://github.com/hzeller/rpi-rgb-led-matrix) �� submodule �Ƃ��Ďg���Ă��܂�

## �r���h�菇

�ꉞ�r���h�ς݂̃t�@�C�����������Ă��܂����A�o���邾���r���h���邱�Ƃ𐄏����܂�

1. �\�ߕK�v�ȃp�b�P�[�W�i���C�u�����j���C���X�g�[�����Ă����܂��傤
  - `sudo apt update -y && sudo apt upgrade -y`
  - `sudo apt install -y python3 python3-pip ffmpeg gcc g++ make libopencv-dev libsdl1.2-dev libsdl2-dev libsdl-mixer1.2-dev libsdl2-mixer-dev`
  - `sudo pip3 install youtube-dl`
2. make �Ńr���h���܂�
  - `make`
  - �r���h�Ɏ��s����ꍇ�A���C�u���������܂��C���X�g�[���o���Ă��Ȃ��\��������܂�
3. make install �Ńp�X�̒ʂ����ꏊ�ɃC���X�g�[�����܂�
  - `sudo make install`
  - ������ sudo �����܂��傤

## �g����

1. ����͍ő� 192 �~ 108 �܂łɃ��T�C�Y���� .mp4 �`���A�����̓T���v�����O���[�g 44100kHz �ɂ��� .mp3 �`���ɂ��A�����t�@�C������ /home/pi/movieplayer/ �ɕۑ����܂��B
   - LED-Movie-Convert ���g���ƁA����E�����̕ϊ����������ł��܂�
   - /home/pi/movieplayer/ �ȊO�ɒu�����Ƃ��\�ł����A���̏ꍇ�� led-movie-player.cc �\�[�X���̋L�q��ύX���Ă�������
2. sudo led-movie-player "[ �g���q�Ȃ��t�@�C���� ]" �Ɠ��͂��Ď��s����ƁA�Đ��ł��܂��B
   - �g���q�Ȃ��t�@�C�����́A�\�� /home/pi/movieplayer/ �ɒu���Ă���t�@�C���̂��̂ɂ��Ă�������
     - �Ⴆ�� /home/pi/movieplayer/ �� TEIDA ZONE.mp4 �� TEIDA ZONE.mp3 ��u�����ꍇ�A[ �g���q�Ȃ��t�@�C���� ] �ɂ� "TEIDA ZONE" �����܂�
     - "TEIDA ZONE" �̂悤�Ƀt�@�C�����ɃX�y�[�X���܂܂��ꍇ�́A�K�� "" �ň͂��悤�ɂ��Ă�������
   - sudo �����Ȃ��ƃG���[�ɂȂ�܂��A�K�� sudo ������ root �Ŏ��s���Ă�������
   - �Ō�̃R�}���h�ȊO�̓f�t�H���g�� 64 �~ 64 �p�l���� 2 ����ɐڑ����Ă��鎖��z�肵�čĐ����܂��B
     - �𑜓x���قȂ�p�l���̏ꍇ�́Aled-movie-player.cc �\�[�X���̋L�q�i 155 �s�ڂ�����j��ύX���Ă�������
   - sudo led-movie-player "[ �g���q�Ȃ��t�@�C���� ]" 0 �Ɠ��͂��Ď��s����ƁA�������[�v�ōĐ����܂��B
   - sudo led-movie-player "[ �g���q�Ȃ��t�@�C���� ]" [ ���[�v�� ] �Ɠ��͂��Ď��s����ƁA�����ɋL�ڂ��ꂽ�񐔂����L�����[�v�ōĐ����܂��B
   - sudo led-movie-player "[ �g���q�Ȃ��t�@�C���� ]" [ ���[�v�� ] [ LED �p�l���̏c�� ] [ LED �p�l���̉��� ] [ LED �p�l���̒���ڑ��� ] [ LED �p�l���̕���ڑ��� ]
�Ɠ��͂��Ď��s����ƁA�����ɋL�ڂ��ꂽ�p�����[�^�ōĐ������s���܂��B
3. Ctrl+C �������ƒ��f�ł��܂��B

## �g�p��
 - ��{�Fsudo led-movie-player "TEIDA ZONE"
 - �������[�v�Fsudo led-movie-player "TEIDA ZONE" 0
 - �L�����[�v�Fsudo led-movie-player "TEIDA ZONE" 15
 - ������S�Ďw��Fsudo led-movie-player "TEIDA ZONE" 1 64 64 2 1

 ## ����
 - 64 �~ 64 �̃p�l���� 2 ����ɂȂ��� LED �p�l����z�肵�Ă��܂��B
   - ���̉𑜓x�̃p�l���ł͓����Ȃ���������܂���
 - ����m�F�� Raspberry Pi 3 B+ ��� Raspbian �ōs���Ă��܂��B
   - ����������ƁA���̃��Y�p�C�ł͂��̂܂܂̃R�[�h�ł͓����Ȃ��A�Ƃ����������邩������܂���
 - �R�}���h���C������Đ����܂��BGUI �͂���܂���B
   - ���Y�p�C���̂���͂Ȃ��߁AGUI ������Ƃ������ĕ`�悪�d���Ȃ邻���c�i rpi-rgb-led-matrix �H���j
 - �낭�Ɍ��؂����Ă��Ȃ������p�\�t�g�i���X�����i�Ŕz�z�������Ȃ��������ǖܑ̖����̂Łc�j�ł��B���̊��œ������͔����c
 - �\�[�X�ɕύX����������A���R�ł����ăr���h���Ȃ��ƕύX�͔��f����܂���B
 - ���܂� Ctrl+C �����Ă����悪���ꑱ����ꍇ������܂��i�v���Z�X�����œ������ςȂ��ɂȂ��Ă��܂��j�c
   - ���̏ꍇ�́A`ps aux | grep "led-movie-player (�N�I�[�e�[�V�����Ȃ��g���q�Ȃ��t�@�C����) (���[�v�w�肵���ꍇ�͓����)" | grep -v grep | awk '{ print "sudo kill -9", $2 }' | sh`
     - ��Fps aux | grep "led-movie-player TEIDA ZONE 0" | grep -v grep | awk '{ print "sudo kill -9", $2 }' | sh
   - �Ǝ��s����Ƌ����I���o����͂��ł��B

## ����E�����̃G���R�[�h

�O�q�̒ʂ�ALED �p�l���ōĐ������铮��≹���́A�\�� LED-Movie-Player �ōĐ��ł���`���ɕϊ�����K�v������܂��B

 - ����
   - .mp4 �`��
   - �ő�ł� 192 �~ 108 �Ƀ��T�C�Y����
     - 720p �Ƃ��̓�������̂܂ܓ˂����ނƏd�����ăX���[�Đ���ԂɂȂ�܂�
     - 160 �~ 90 �̕������Y�����܂���
       - 192 �~ 108 �̓M���M���ł�
   - ������ mp3 ����Đ�����̂ł����Ă��Ȃ��Ă���
 - ����
   - .mp3 �`��
   - �r�b�g���[�g��128kbps�ɂ���
     - 192kbps �Ƃ��ł������邩������܂��񂪈ꉞ
   - �T���v�����[�g��K��44100kHz�ɂ���
     - 48000kHz ���Ɖ����o�O��c

LED-Movie-Convert �𗘗p���āAYouTube ���̓���⃉�Y�p�C��ɂ��铮��������ŕϊ������邱�Ƃ��ł��܂��B  
�o���邾��������𗘗p���鎖�������߂��܂��i��q�j

�蓮�ŃG���R�[�h����ꍇ�Affmpeg �ŗ\�ߕϊ����Ă����܂��傤�i Windows �ł������̂ŁAPC �ŃG���R�[�h���ă��Y�p�C�Ɉڂ��̂�����ł��j

����̃G���R�[�h�R�}���h��: ffmpeg -i "Movie.mp4" -vf scale=192:108 "Movie-led.mp4"  
�����̃G���R�[�h�R�}���h��: ffmpeg -i "Movie.mp3" -vn -af dynaudnorm -ac 2 -ar 44100 -ab 128k "Movie-led.mp3"

�ϊ����I�������A�ϊ���̃t�@�C�������Y�p�C�� /home/pi/movieplayer/ �ȉ��ɃR�s�[���Ă��������B  
�O�q�̒ʂ�Aled-movie-player.cc �\�[�X���̋L�q��ύX���A�t�H���_��ύX���邱�Ƃ��\�ł�

## LED-Movie-Convert

### ����

 - �o�b�`�t�@�C���ł� (�p�X�̒ʂ����ꏊ�ɒu���΃R�}���h���l�Ɏ��s�ł��܂�)
 - sudo make install����ۂɂ��łŃp�X�̒ʂ����ꏊ�ɃC���X�g�[������܂�
 - ���Y�p�C�̃X�y�b�N���Ⴂ�֌W�ŃG���R�[�h�Ɏ��Ԃ�������܂��A�C���ɑ҂��܂��傤
 - �G���R�[�h���͕��ׂ����ɍ����Ȃ�̂ŁA�G���R�[�h���� LED-Movie-Player �ōĐ������悤�Ƃ���ƃX���[�Đ��ɂȂ��Ă��܂���������܂�
   - (���Y�p�C�̐��\���Ⴂ���� LED �p�l���ւ̕\�����������ۂ̍Đ��X�s�[�h�ɒǂ��t���Ȃ�)

### �g����
 - �������c �\�[�X�����w�肵�܂� ( URL or �t�@�C���p�X)
 - �������c �\�[�X�̎�ނ�I�����܂� ( URL ����_�E�����[�h����Ȃ� download �A�t�@�C���p�X����Ȃ� file )
 - ��O�����c �t�@�C������ݒ肵�܂� (�t�@�C�����𓮉於 or �t�@�C����������Ȃ� auto �A����ȊO�Ȃ�g���q�Ȃ��̃t�@�C����������)
 - ��l����(�I�v�V����)�c ����̉������w��i��: 160 �j
 - ��܈���(�I�v�V����)�c ����̍������w��i��: 90 �j

### �g�p��

 - YouTube ������_�E�����[�h����ꍇ
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download auto
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE"
   - led-movie-convert https://www.youtube.com/watch?v=3yuEZ103aNY download "RED SUMA ZONE" 192 108
 - ���[�J���t�@�C������ϊ�����ꍇ
   - led-movie-convert "/home/pi/TEIDA ZONE �y�ѓc�w�~TEI ZONE�z.mp4" file auto
   - led-movie-convert "/home/pi/TEIDA ZONE �y�ѓc�w�~TEI ZONE�z.mp4" file "TEIDA ZONE"
   - led-movie-convert "/home/pi/TEIDA ZONE �y�ѓc�w�~TEI ZONE�z.mp4" file "TEIDA ZONE" 160 90

## �ӎ�

�������̋L���i https://qiita.com/cstoku/items/eb17d111225d3e35ec61 �j�̒��҂� cstoku ����A�f���炵���R�[�h��{���ɂ��肪�Ƃ��������܂��c   
�i�������ł͂܂������Ȃ��R�[�h�ł����E�\�������͂قڂ��̂܂܂ł��j

## License
[MIT Licence](LICENSE.txt)

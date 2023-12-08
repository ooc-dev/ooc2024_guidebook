# OOC2024ガイドブック

## 概要説明
公式ガイドブックのリポジトリです。

## この本の目的

実行委員会原稿とスポンサー記事をここで書きます。

原稿は、/markdownのフォルダに入れてください。コンバートします。
変換済は/markdown/oldに移動します。

## スケジュール
2024年3月24日　イベント当日

2024年3月初旬　入稿予定日

2024年2月日　公称最終締め切り

2024年1月30日　公称締切

## 執筆にあたってのお願い
Confrictが発生した場合は、解決お願いします。

push -f等はやめましょう。（歴史を書き換えてはいけません。

相談事：
Issue立ててください。

## Re:VIEWの書き方

[Re:VIEWチートシート](https://gist.github.com/erukiti/c4e3189dda179a0f0b73299fb5787838) を作ってみたので、参考にしてみてください。


### Dockerを使う方法

Dockerを使うのが一番手軽です。

```sh
$ docker run --rm -v `pwd`:/work vvakame/review /bin/sh -c "cd /work/articles ; review-pdfmaker config.yml"
```

### bat/shでビルド
build-print.bat当たりを叩くと/src内にpdfができるよ！

### 権利

ベースには、[TechBooster/ReVIEW\-Template: TechBoosterで利用しているRe:VIEWのテンプレート（B5/A5/電子書籍）](https://github.com/TechBooster/ReVIEW-Template) を使っています。

  * 設定ファイル、テンプレートなど制作環境（techbooster-doujin.styなど）はMITライセンスです
    * 再配布などMITライセンスで定める範囲で権利者表記をおねがいします

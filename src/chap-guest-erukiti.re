= 生成AI最前線で戦い抜くための設計上の試み

//flushright{
株式会社Algomatic 佐々木俊介 @erukiti
//}

どうもerukitiと申します。生成AIでできることすべてをやり尽くす株式会社Algomaticで、シゴラクAIを開発しています。

この章では、状況が刻一刻と変わるとても変化の大きい世界で、それに対応するための設計の話について書いています。ただし、実のところ、そんなに特別なことはしていません。

なお、@<chapref>{chap-sponsor-algomatic1}ではシゴラクAI本体の設計で工夫したことを取り上げていて、この記事ではその後の試みについて書いています。

== 概要

生成AIの市場は十年以内で数十〜百兆円規模になるといういくつかの試算@<fn>{chap-erukiti-genai-market-size}があるものの、現時点で大きく成功しているプロダクトは、まだOpenAIやMicrosoft（GitHub）など、限られているという状況です。生成AI時代における最適なUXがどうなるのか？いくつか仮説は考えられますが、まだまだ探求が始まったばかりです。つまり、不確実性が極めて大きいのです。

//footnote[chap-erukiti-genai-market-size][試算のうち https://www3.nhk.or.jp/news/html/20231221/k10014294931000.html では2030年に30兆円とされています。]

また、生成AIに関する大ニュースが一週間で何個も重なることも珍しくありません。つまり、変化がやたらと早いというという特徴もあります。

弊カンパニーでは@<chap>{chap-sponsor-algomatic1}にもあるようにシゴラクAIというサービスを開発・運営していますが、同時に全然違うような試みを探求することも少なくありません。

まとめると、生成AIという不確実性が極めて高く、変化がやたら早い世界で、シゴラクAIというすでに稼働しているサービスを維持・開発しつつ、それとは完全に別軸でドラスティックな実験をするための設計が必要となります。

 * 不確実性が高い
 * 変化が早い
 * 既存のサービスを維持する必要がある
 * 実験的な試みをする必要がある

== RAG

さて話は変わりますが文章生成AIではRAG@<fn>{chap-erukiti-rag}という技術がずっと流行っています。検索した情報を元に生成する技術です。ただし検索と言っても一般的な検索エンジンではなく、ドキュメントを用意してそれを元に検索インデックスを作成しておくというものです。
//footnote[chap-erukiti-rag][RAGはRetrieve Augmented Generationの略です。]

社内ドキュメントを用意すれば社内の知識を元にAIが答えられますし、ユーザー向けサービスのドキュメントなら、ユーザーからの問い合わせにAIが答えます。またWikipediaなどを用意することで一般的な応答を強化することもできます。

このような応用例の広さから、現状で生成AIに取り組んでいるほとんどの会社でRAGに取り組んでいるのでは？というくらいホットなトピックです。弊社も例外ではなく、RAGに関する実験的な取り組みが行われています。

実際にChatGPTなど文章生成AIを触ってみた人はわかるかもしれませんが、文章生成AIに知識を問い合わせてみるとでたらめな文章@<fn>{chap-erukiti-halucination}を生成しがちです。RAGはドキュメントを与えることによって正しく事実やコンテキストに沿った回答をさせられるため、会話の精度を上げるためにはRAGは非常に有用です。
//footnote[chap-erukiti-halucination][いわゆるハルシネーションというやつです。]

RAGについてもう少し踏み込んでみましょう。RAGではユーザーが入力した文章を元に検索クエリを作成し検索エンジンからデータを取得します。そしてそのデータを元に文章を生成します。

 * ユーザーが入力した文章から検索クエリを作成する
 * 検索クエリを元に検索エンジンからデータを取得する
 * 取得したデータから実際に使うデータを選ぶ
 * そのデータを元に文章を生成する

行程を分解すると、おおよそこのような流れになります。これに加えてRAGでは全文検索エンジンをそのまま使う事はほとんどなく大抵はベクタ検索と呼ばれる技術が使われるなど、検索インデックス作成にも工夫が必要です。

精度を上げるためには、これらの行程のすべてで地道な工夫を重ねる必要がありますが、この記事はRAGについて理解するためのものではないため簡単に触れておきます。

 * 検索エンジンにはベクタ検索というような技術を用いたり、ベクタ検索と全文検索のハイブリッドを用いる
 * ユーザーのクエリを、より検索しやすいように書き換えて、検索クエリとする
 * 検索クエリを複数生成して、複数の結果を取得し、それらを融合する

これらは検索時の工夫ですが、そもそも検索インデックスを作る段階で行える工夫もあります。

 * 検索インデックスを作成する時点で、ドキュメントを分析しておく
 * 検索インデックスの作り方を工夫することでより適切に検索できるようにする
 * PDFはそもそもマシンリーダブルとは言いがたい仕組み@<fn>{chap-erukiti-pdf}なため、読み込みを工夫する

//footnote[chap-erukiti-pdf][PDFは紙に印刷するためのものであり、座標という概念に強く結びついたものです。素性の良いPDFですらヘッダやフッタといった邪魔なものが付与されていてノイズになります。一番最悪なものでは画像がそのまま埋め込まれているためテキスト情報がないケースもあります。]

他にも様々な工夫の余地があります。興味があるひとはLlamaIndex@<fn>{chap-erukiti-llamaindex}というOSSを触ってみるといいでしょう。
//footnote[chap-erukiti-llamaindex][https://www.llamaindex.ai/]

ということで一通りの前提を説明したところで、本題に入ります。シゴラクAIを開発しつつ、シゴラクAIと独立したリポジトリでRAGの実験をしているというのが、この記事の本題です。

== ShigoRAG

シゴラクAIのRAG実装のみを抜き出したリポジトリであるShigoRAG@<fn>{chap-erukiti-shigorag}は、シゴラクAI本体の変更と切り離してRAGの精度向上を行うために生まれました。

//footnote[chap-erukiti-shigorag][当初shigoraku-ragという名前でしたが、「シゴラクのRAGって言いづらいし、シゴRAGでいいじゃん」みたいなやりとりから生まれたネーミングです。]

シゴラクAIは元々、二週間で開発され、そのあと環境構築などを経てリリースされたプロダクトです@<fn>{chap-erukiti-shigoraku-start}。その後の開発では、メンバーも増え、ある程度大きなコードベースに成長しました。RAG機能はシゴラクAIにおいては6月頃から開発が開始したもので長く使われていますが、途中から精度向上を本格的に取り組むことになりました。

//footnote[chap-erukiti-shigoraku-start][スタートアップにありがちな速度感ですね。]

精度向上のために実験を繰り返すのを、当初は本家シゴラクAIのコードベース上でやってたのですが、どうしてもやりづらさがあったので分離した形です。組織以外の理由でコードベースを分離するというのもなかなかない気がしますが、疎結合にするためにはリポジトリの分離をするのは一つありだなーと今回しみじみ思ったところです。

ぶっちゃけ人が増えすぎてコードも巨大になって、初期の生みの親である筆者の認知限界を超えてしまっていたので、切り離してリスタートしたかったというのがあります。

ShigoRAGは、シゴラクAIから引き続いてTypeScriptで書いていますが、筆者がtsconfig.jsonと格闘したくなかったのでNode.jsではなくDenoを前提としています。

また、シゴラク本体よりもclass basedなOOPを使っています。シゴラク本体ではフロントエンドとは異なる別のバックグラウンドを持ってる人が割といるので、最初からclassで書いた方がコードの統一感は持たせやすいという知見が得られたからです。

慣れていない人はTypeScriptのアンチパターンを踏み抜いてしまう傾向がありますが、他の言語と共通するものを採用すると改善されるようです。

設計をする上での基本はどんなプロダクトでも変わることはないでしょう。疎結合・高凝集性だとか、知識が不要に漏れ出ないこと、適切な制約を持つこと、などです。

前述の通りShigoRAGは元々RAGの実験を行うために分離したものです。コマンドライン上でDenoを使って動かす事を前提としています。RAGではそれぞれの処理フェイズごとに工夫の余地があるため、それぞれのフェイズの独立性は高い方が望ましいです。

ただし個々のフェイズをすべて組み合わせて実行できるわけでもありません。そこで厳密な独立性は考えないことにしました。

大雑把にShigoRAGで回答をするときの動きを説明すると、まず@<code>{Context}を作成し動作に必要な情報を格納してから@<code>{RAG}クラスをインスタンス化してから@<code>{answer}メソッドを呼び出すだけです。

この@<code>{Context}にはRAGの要である検索エンジンをたたくコードや、文章生成AIのAPI化である OpenAI Chat Completions API をたたくコードなんかも含まれています。あまり明確にレイヤー構造を意識していたわけではないものの、検索の仕組みとRAGの実装を分離したかったというのはあります。

@<code>{RAG}クラスは外部から見たときは@<code>{answer}メソッドしか見えませんが、内部的にはさらに3つの工程に分解されます。

 * ユーザーのクエリを前加工
 * 前加工したクエリを元に検索する
 * 検索してきたテキストを元に回答を生成

これらの工程は先ほども書いたように、必ずしも組み合わせて使えるわけではないため、明確に独立した構造にはしていません。
ここを必要以上に複雑にすると、実験をする上で面倒になると考えたからです。
そのためprotected methodにしている程度です。

== 最後に

さてこのShigoRAGという試みはどうなったか？というと、定量的に計測したわけではありませんが、シゴラク本体でやっていた頃よりRAGの検証は捗っています。開発のしやすさにより活発にcommitが行われていてとても良い状態になっています。

探索が必要なプロダクトなので、今後もこのような小さな試みを繰り返していくことになるでしょう。捨てやすく、疎結合にした小さなモジュールを組み合わせるという方法が有効なはずですが、今後これを繰り返して、さらに考えを変えるかもしれません。

　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/erukiti.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        @erukiti https://twitter.com/erukiti\\
        株式会社AlgomaticでシゴラクAIを開発しているTS系JSエンジニャー
    \end{minipage}
    \hspace{1ex}
//}

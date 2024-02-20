= DDDもスクラムも当たり前な開発組織のその先へ

//flushright{
株式会社ログラス　飯田意己
//}

//note{
当稿は Loglass TECH TALK #1 の内容を元に、Object-Oriented Conference 2024 ガイドブック向けに加筆・修正したものとなります。
https://www.youtube.com/watch?v=IxQvELUvQgc
//}

== ログラスにおけるDDDの取り組み

=== 戦略的DDDの実践

ログラスでは創業当初よりドメイン駆動設計（以下DDD）に戦略的に取り組んできました。  

ログラスがDDDに取り組んできた背景として最も大きなものは「業務が非常に複雑で、ユーザーである経営企画がエンジニアにとって非常に遠い存在である」ことが挙げられます。

エンジニアが経営企画の業務知識が浅いままに複雑な要件を実装していけば、そのシステムはバグだらけの使えないシステムになってしまうことは想像に難くありません。

ログラスはCEOの布川が元経営企画だったこともあり、ドメインエキスパートとして経営企画の業務を直接エンジニアに伝えられるという、DDDを実践しやすい状況にありました。

そこで創業時からCEOをドメインエキスパートとして、開発チームと一緒にドメインモデリングを行い、業務理解からドメインモデルを整理し実際の実装に繋げていくというサイクルを作ることができました。

//image[domainmodeling][創業時のドメインモデリングの様子][scale=0.5]

この取り組みの中で「予算と実績と見込みはどう違うのか？」「勘定科目、補助科目の設定は誰ができるべきなのか？」といった議論を行うことができ、このプロセスに大きな価値がありました。

このようなドメインモデリングの営みはチームが複数になった今でも行われておりログラスの開発チームのカルチャーとして根付いています。

結果としてモデリングのアウトプットもストック情報として積み上がっています。

//image[fig-domainmodel][ドメインモデル図][scale=0.7]

=== 戦術的DDDの実践

アーキテクチャとしても初期に設計したアーキテクチャで現在まで開発を続けています。

//image[architecture][アーキテクチャ概要][scale=0.9]

ログラスではSpring Bootを利用しており、レイヤーごとにGradleのProjectで管理しています。
こうすることでProject間の依存関係を定義することができ、レイヤーごとの責務をコードレベルで守ることができます。

また、設計標準という形でDDDに関するもの以外も含めて実装のベストプラクティスをドキュメントとして運用しています。

//image[basic][設計標準][scale=0.7]

設計標準については以下のように定義しています。

 * 設計における標準を定めます。
 * 定められて守るべき「規約」ではなく、それを軸として考えるための「標準」です。
 ** 「標準」を参考にしながら全員がよりよい設計を追求し、
      さらにその検討過程を設計標準に残すことで、
      チームの資産として育てていくことを目指しています。
 ** 結論だけではなく、議論の過程も残すことで後から意図を汲み取りやすくします。

このようにただルールを守るのではなく、エンジニア全員が考える余地を作りフェーズごとに最適なルールにアップデートしていく思いが込められています。

== DDDを支える組織設計

ログラスでは創業時より、プロダクトがビジネスの根幹にあり、不確実性が高く探索と軌道修正を小さなサイクルで繰り返す必要があったため、スクラムで開発を進めてきました。

DDDはそもそも、イテレーティブな開発やエンジニアとドメインエキスパートが密接に関わっているようなアジャイルな開発プロセスが想定されています。

したがってDDDを推進する上でもスクラムチームが熟達し、アジャイルマニフェストにある価値@<fn>{1}を追求していくことが非常に重要です。

 * 包括的なドキュメントよりも動くソフトウェア
 * 計画に従うことよりも変化への対応
 * プロセスやツールよりも個人と対話
 * 契約交渉よりも顧客との協調

//footnote[1][https://agilemanifesto.org/iso/ja/manifesto.html]

まず、創業時の1チームでしっかりと顧客に向き合い、業務理解を深めながらドメインモデリングを行い、お客様に価値が届いているか検証を行う、という一連のサイクルの成功体験を積むことが非常に重要だと考えていました。  
チーム分割をしていく際にもこの初期の成功体験を持った人たちのイメージが基準となるからです。

== スクラムの現在地

最初のスクラムチームの組成から約2年が経過し、現在は1つのプロダクトを3チームで開発しています。

//image[001][システムと価値の関係性][scale=0.7]

チームの分割単位は機能領域ごとでチームを分けており、この分割には以下の観点があります。

 * 技術的複雑性をマネジメントしやすくする
 * ドメイン層のコードが分かれており、領域内に閉じる価値についてはアジリティを出しやすい

ログラスの機能領域はそれぞれに異なる技術的複雑性があり、チーム領域を跨いだ際に一定のキャッチアップコストが発生するという特性があります。

この特性を扱うためには領域ごとにチームを安定化させることでナレッジを育てることができます。  
また、領域に閉じた価値においてはチームで独立して開発を進められるため高速にデリバリーできるというメリットがありました。

しかしながら、上段にあるような領域を跨ぐような大きな価値を作り出すための開発は当然のことながらチーム間連携が必要になり、ドメインモデリングの難易度も上がります。  
現在はスクラムのスケールは行っておらず、各チームのスクラムをそれぞれで進化させてきたという背景があるため、チーム間連携の模索が必要な状況です。

== スタートアップのスケールにおいて必要なこと

スタートアップでは事業成長も組織成長も高速に進んでいきます。

初期フェーズに採用した設計方針が良ければ良いほど長く走れますが、必ず寿命があります。  
現在の技術戦略の耐用年数を考慮し次の戦略を探索していくことが、高速で走り続けるスタートアップの次のフェーズを作り出すために必要な営みだと考えています。

=== 技術的卓越性の推進によって未来を切り開く

技術は手段であり、ビジネスの課題を解決するためにバランスが取って活用している状態が望ましいですが、一方で技術を現状維持でマネジメントしていくと陳腐化によってビジネス課題に追いつかなくなっていく状態が発生します。

したがってビジネスの課題を解決し続けられる状態を作り出すためには現状維持よりかは多くの投資が必要です。  
この投資によって作られる技術的卓越性こそがビジネス的な強みとなり、その後の成長を作り出す土台となります。

ログラスでは今後大規模なデータモデリングや関数型データエンジニアリングなどSystem of Insightにとって非常に重要な領域への投資を進めていきます。

DDDもスクラムも当たり前な開発組織のその先の世界を一緒に作っていただける方を募集しています。

//indepimage[qr][https://job.loglass.jp/]


　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/loglass.jpg}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        飯田意己@ysk\_118　https://twitter.com/ysk\_118\\
        株式会社 ログラス
    \end{minipage}
    \hspace{1ex}
//}
ログラスでシニアエンジニアリングマネージャーをしています。

//noindent
アーキテクチャと組織の設計をいいかんじにすることに関心があります。
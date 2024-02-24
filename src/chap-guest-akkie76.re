= レビューガイドラインを導入してみよう！

//flushright{
佐藤 晶彦 @akkiee76
//}

== レビューガイドラインの導入の背景

現在、私のチームは技術育成を目的として、レビューガイドラインを導入しています。特に若手エンジニアが抱える技術的な課題に対処するため、レビュー指摘を類型化し、メンバーの弱点を定量化するガイドラインを作成しコードレビューに臨んでいます。

== レビューガイドライン 7 つの観点

レビューガイドラインの作成にあたっては「Google's Engineering Practices documentation」@<fn>{reviewguideline}を参考にし、7 つの観点を設けています。

//footnote[reviewguideline][https://google.github.io/eng-practices/]

//table[table1][レビューガイドライン7つの観点]{
No	観点	概要
------------
1	Design	アーキテクチャ、設計
2	Simplicity	理解容易性
3	Naming	クラス、メソッド、変数名などの命名
4	Style	コードスタイル
5	Functionality	機能（要件）を充足しているか
6	Test	テストの記述、パターンが適切
7	Document	コメント、ドキュメントに関連
//}

特に、No.1 ~ No.4は、オブジェクト指向の観点で非常に重要な観点といえます。ここからは具体例を踏まえて、実際の使用例を紹介したいと思います。

//pagebreak

==== Design（設計）

//emlist[定義]{
アプリケーションとして一貫性のあるアーキテクチャ、設計となっているか。また、アプリケーションにとって適切な責務、振舞いになっているか。
//}

Design では、主に以下のケースが指摘の対象になります。アーキテクチャ原則違反はアプリケーションで導入しているアーキテクチャを前提にしており、これに違反している場合は指摘の対象になります。他にも以下のような基本的な SOLID 原則に反するような場合も指摘の対象になります。

 * アーキテクチャ原則違反
 * 単一責任の原則違反
 * 密結合
 * 低凝集
 * DRY 原則違反 etc.

==== 単一責任の原則違反の例

以下のコードでは、@<b>{add()} で2つのバリデーションと2つのビジネスロジックが実装されており、単純に「商品を追加する」以外の責務を1つの関数の責務として行われています。この関数自体複数の責務を持ってしまっているため(責務超過)、Design 指摘の対象となります。

　

//emlist[多重責務の関数例]{
class SpecialDiscountManager {

    // 商品の割引に関するビジネスロジッククラス
    lateinit var manager: DiscountManager

    fun add(product: Product): Boolean {
        if (product.id < 0) { // バリデーション 1
            throw IllegalArgumentException()
        }
        if (product.name.isEmpty()) { // バリデーション 2
            throw IllegalArgumentException()
        }
        val tempPrice: Int = if (product.canDiscount) { // 条件分岐 1
            manager.totalPrice + manager.getDiscountPrice(product.price)
        } else {
            manager.totalPrice + product.price
        }
        return if (tempPrice < 3000) { // 条件分岐 2
            manager.totalPrice = manager.discountProducts.add(product)
            true
        } else {
            false
        }
    }
}
//}

このように１つの関数に複数の責務が実装されるような場合、なるべく関数の責務を分離することでテスト容易性も高まるため、望ましい設計といえるでしょう。

　

//emlist[責務分離の実装例]{
class SpecialDiscountManager {

    /** 商品の割引に関するビジネスロジッククラス */
    lateinit var manager: DiscountManager

    /**
     * 商品を追加する
     * 
     * @param product 商品
     */
    fun add(product: Product) {
        // 不正な商品をバリデートする
        validateInvalidProduct(product)
        // 商品を追加する
        if (canAdd(product)) {
            manager.totalPrice = manager.discountProducts.add(product)
        }        
    }

    /**
     * 不正な商品をバリデートする
     * 
     * @param product 商品
     * @throws IllegalArgumentException 不正な商品である場合の例外
     */
    private fun validateInvalidProduct(product: Product) {
        if (product.id < 0 || product.name.isEmpty()) {
            throw IllegalArgumentException()
        }
    }

    /**
     * 商品が追加可能か
     * 
     * @param product 商品
     * @return 追加可能かどうか
     */
    private fun canAdd(product: Product): Boolean {
        val tempPrice = getTempPrice(product)
        return tempPrice >= 3000
    }

    /**
     * 商品の一時金額を取得する
     * 
     * @param product 商品
     * @return 商品の一時金額
     */
    private fun getTempPrice(product: Product): Int {
        return if (product.canDiscount) {
            manager.totalPrice + manager.getDiscountPrice(product.price)
        } else {
            manager.totalPrice + product.price
        }
    }
}
//}

==== Simplicity（理解容易性）

//emlist[定義]{
可読性のあるコーディングになっているか。処理ができるだけシンプルな振る舞いになっているか。
//}

以下のようにコードが複雑になる場合、Simplicity の指摘対象になります。

 * ネストが深い if 文
 * 複雑な三項演算子文
 * 冗長な SQL
 * stream, filter, map を多用した Object 整形文 etc.

例えば、以下のようなif分は指摘の対象になります。

　

//emlist[if - else 分岐が多い実装例]{
val powerRate: float = member.powerRate / menber.maxPowerRate
var currentCondition: Condition = Condition.DEFAULT
if (powerRate == 0) {
    currentCondition = Condition.DEAD
} else if (powerRate < 0.3) {
    currentCondition = Condition.DANGER
} else if (powerRate < 0.5) {
    currentCondition = Condition.WARNING
} else {
    currentCondition = Condition.GOOD
}
return currentCondition
//}

実際のレビューコメントでは、以下のようにネストを解消するように指摘をする場合などに使用します。

//pagebreak

//emlist[早期リターンへの改善例]{
val powerRate: float = member.powerRate / menber.maxPowerRate
if (powerRate == 0) {
    return Condition.DEAD
}
if (powerRate < 0.3) {
    return Condition.DANGER
}
if (powerRate < 0.5) {
    return Condition.WARNING
}
return Condition.GOOD
//}

==== Naming（命名）

//emlist[定義]{
変数やクラス、メソッドに責務を意図した明確な名前が付けられているか。英語文法に誤りがないか。typoもこれに含まれる。
//}

このような命名に関する指摘をする場合に使用します。

 * 振舞いと一致しない変数名、関数名
 * 責務と一致しない関数名
 * 英文法の誤り etc.

==== Style（コードスタイル）


//emlist[定義]{
コードスタイルが言語仕様に準拠しているか。
//}

コードスタイルは、言語仕様やフレームワークに準拠することが基本です。公式にガイドラインが公開されている場合、これに反することは避けるべきです。

 * 静的解析違反
 * 不適切なアクセス修飾子
 * 表記違反(スネーク、キャメルなど) etc.

　

　

　

==== Functionality（機能要求）


//emlist[定義]{
システムとして外部仕様を充足しているか。作者が意図した通りの振る舞いであるか。また、システムの通信量、パフォーマンスに懸念がないか。
//}

主な観点としては、外部機能など仕様を充足しているかという点が対象になります。
また、パフォーマンスやセキュリティといった非機能要件に懸念がある場合も指摘の対象となります。

 * 外部機能が充足していない（不具合）
 * 概要設計書のフローチャートと異なるフローになっている
 * パフォーマンスの低下が考えられる
 * 不要データを送信している etc.

　

==== Test（テスト）


//emlist[定義]{
システムとして適切な自動テストを兼ね備えているか。自動テストの内容で品質を担保できているか。また、システムを担保するパラメータ群を備えているか。
//}

テストコードが期待になっていない場合や、テストでのパラメータに考慮漏れがある場合などテストに関連する事項が指摘の対象になります。

 * 対象のメソッドがテストされていない
 * テストパターンが網羅できていない（パタメータテスト、閾値テストの不足）
 * 分岐がパターンが網羅されていない
 * 実装上宣言している静的定数値が直接ハードコードされている
 * アーキテクトに準じたテストになっていない

//pagebreak

==== Document（文章）

//emlist[定義]{
ソースコード上に記載されているdoc、コメントが適切な内容であるか。また、関連するドキュメントは更新されているか。その内容は適切か。
//}

ソースコードに関連するコメントだけでなく、プロジェクトで管理している関連ドキュメント（README）も対象になります。

 * 関連ドキュメントの更新漏れ（READMEなど）
 * docやコメントの内容が不適切、内容が不適切

== コードレビューでの指摘方法

コードレビューでは上記の 7 つの観点に加えて、指摘修正の要否を4つの類型に分けてコメントをします。

//table[table2][修正要否]{
観点	概要
------------
MUST	PR、MRをマージするためには必ず修正が必要
SHOULD	修正なしにマージすることはできるが、リリースまでには修正が必要
IMO	レビューアー観点の意見。修正不要
NITS	IMOより細かい意見など。修正不要
//}

コードレビューでマージするために必要な修正は MUST 指摘となります。 MUST と SHOULD の使い分けは難しい部分もありますが、SHOULD での指摘事項は別途リリースまでに対応するなど、レビューアーを相談して対応を進めます。一方、IMO や NITS は修正は不要ですが、修正しない場合はその旨をコメントに返信してもらい、コメントを閉じてからマージする運用をしています。具体的な運用方法はチームによって異なるので、チームにあった方法で運用するのが良いと思います。

==== 具体的な利用方法

実際にコードレビューをするとき、上記の 7 つの観点と修正の温度感をこのように交えた Prefix を付与してコメントします。

　

//emlist[指摘例]{
MUST(Design):ドメインロジックが Controller クラスに実装されてます。 domain 層の対象 package に新しくクラスを作成して実装を移してください。
//}

== 最後に

このように、レビューコメントを分析し PDCA サイクルを回すことで、技術的課題へのアプローチ方法も見えてきます。また、コードレビューでオブジェクト指向が学べるといった恩恵を受けることできるので、ぜひチームに合ったレビューガイドラインを作成してみてはいかがでしょうか。

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/akkiee76.jpg}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        佐藤 晶彦 @akkiee76 https://twitter.com/akkiee76\\
		iOS, Android アプリの開発を中心に行なっています。懇親会で皆さんとお話しできることを楽しみにしています。
    \end{minipage}
    \hspace{1ex}
//}
= コードレビューガイドラインの導入と背景

//flushright{
akkie76@akkie76
//}

現在、私のチームは技術育成を目的として、コードレビューガイドラインを導入しています。特に若手エンジニアが抱える技術的な課題に対処するため、レビュー指摘を類型化し、メンバーの弱点を定量化するガイドラインを作成しコードレビューに臨んでいます。

実際のガイドライン作成では、Google が公開するレビューガイドライン@<fn>{reviewguideline}を参考にし、7 つの観点を設けています。

//footnote[reviewguideline][レビューガイドライン(https://fujiharuka.github.io/google-eng-practices-ja/ja/review/reviewer/standard.html)]

//table[table1][レビューガイドライン7つの視点]{
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

== Design（設計）

=== 定義

_アプリケーションとして一貫性のあるアーキテクチャ、設計となっているか。また、アプリケーションにとって適切な責務・振舞いになっているか。_

=== 具体例

基本的には、以下のケースが指摘の対象になります。アーキテクチャ原則違反はアプリケーションで導入しているアーキテクチャを前提にしており、これに違反している場合は指摘の対象になります。他には、以下のような基本的な SOLID 原則に反するような場合は指摘の対象になります。

 * アーキテクチャ原則違反
 * 単一責任の原則違反
 * 密結合
 * 低凝集
 * DRY 原則違反 etc.

==== 単一責任の原則違反のケース

以下のコードでは、`add()` で２つのバリデーションと2つのビジネスロジックが実装されており、単純に「商品を追加する」以外の責務を１つの関数の責務として行われています。この関数自体複数の責務を持ってしまっている（責務超過）ため、Design 指摘の対象となります。

//list[list1][kotlin:SpecialDiscountManager.kt]{
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

このように１つの関数に複数の責務が実装されるような場合、以下のようになるべく関数の責務を分離することが望ましいでしょう。

//list[list2][kotlin:SpecialDiscountManager.kt]{
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

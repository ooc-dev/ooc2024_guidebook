## コードレビューガイドラインの導入と背景

現在、私のチームは技術育成を目的として、コードレビューガイドラインを導入しています。特に若手エンジニアが抱える技術的な課題に対処するため、レビュー指摘を類型化し、メンバーの弱点を定量化するガイドラインを作成しコードレビューに臨んでいます。

実際のガイドライン作成では、Google が公開する[レビューガイドライン](https://fujiharuka.github.io/google-eng-practices-ja/ja/review/reviewer/standard.html)を参考にし、7 つの観点を設けています。

| No | 観点 | 概要 |
| - | - | - |
| 1 | Design | アーキテクチャ、設計 |
| 2 | Simplicity | 理解容易性 |
| 3 | Naming | クラス、メソッド、変数名などの命名 |
| 4 | Style | コードスタイル |
| 5 | Functionality | 機能（要件）を充足しているか |
| 6 | Test | テストの記述、パターンが適切 |
| 7 | Document | コメント、ドキュメントに関連 |

特に、No.1 ~ No.4は、オブジェクト指向の観点で非常に重要な観点といえます。ここからは具体例を踏まえて、実際の使用例を紹介したいと思います。

### 1. Design（設計）

#### 定義

_アプリケーションとして一貫性のあるアーキテクチャ、設計となっているか。また、アプリケーションにとって適切な責務・振舞いになっているか。_

#### 具体例

基本的には、以下のケースが指摘の対象になります。アーキテクチャ原則違反はアプリケーションで導入しているアーキテクチャを前提にしており、これに違反している場合は指摘の対象になります。他には、以下のような基本的な SOLID 原則に反するような場合は指摘の対象になります。

* アーキテクチャ原則違反
* 単一責任の原則違反
* 密結合
* 低凝集
* DRY 原則違反 etc.

 ##### 単一責任の原則違反のケース

以下のコードでは、`add()` で２つのバリデーションと2つのビジネスロジックが実装されており、単純に「商品を追加する」以外の責務を１つの関数の責務として行われています。この関数自体複数の責務を持ってしまっている（責務超過）ため、Design 指摘の対象となります。

```kotlin:SpecialDiscountManager.kt
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
```

このように１つの関数に複数の責務が実装されるような場合、以下のようになるべく関数の責務を分離することが望ましいでしょう。

```kotlin:SpecialDiscountManager.kt
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
```

### 2. Simplicity（理解容易性）
#### 定義

_可読性のあるコーディングになっているか。
処理ができるだけシンプルな振る舞いになっているか。_

#### 具体例

以下のように実装が複雑になる場合、指摘の対象になります。

* ネストが深い if 文
* 複雑な三項演算子文
* 冗長な SQL
* stream, filter, map を多用した Object 整形文 etc.

このように分岐が多いif分は指摘の対象になります。

``` kotlin:Before.kt
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
```

実際のレビューコメントでは、以下のようにネストを解消するように指摘をする場合などに使用します。

``` kotlin:After.kt
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
```

### 3. Naming（命名）
#### 定義

_変数やクラス、メソッドに責務を意図した明確な名前が付けられているか。
英語文法に誤りがないか。typoもこれに含まれる。_

#### 具体例

このような命名に関する指摘をする場合に使用します。

* 振る舞いと一致しない変数名、関数名
* 責務と一致しない関数名
* 英文法の誤り etc.

### 4. Style（コードスタイル）
#### 定義

_コードスタイルが言語仕様に準拠しているか。_

#### 具体例

コードスタイルは、言語仕様やフレームワークに準拠することが基本です。公式にガイドラインが公開されている場合、これに反することは避けるべきです。

* 静的解析違反
* 不適切なアクセス修飾子
* 表記違反(スネーク、キャメルなど) etc.


### 5. Functionality（機能要求）
#### 定義

_システムとして外部仕様を充足しているか。作者が意図した通りの振る舞いであるか。
また、システムの通信量、パフォーマンスに懸念がないか。_

#### 具体例

主な観点としては、外部機能を充足しているかという点が対象になります。
また、パフォーマンスやセキュリティといった非機能要件に懸念がある場合も指摘の対象となります。

* 外部仕様の充足していない（不具合）
* 概要設計書のフローチャートと異なるフローになっている
* パフォーマンスの低下が考えられる
* 不要データを送信している etc.

### 6. Test（テスト）
#### 定義

_システムとして適切な自動テストを兼ね備えているか。自動テストの内容で品質を担保できているか。
また、システムを担保するパラメータ群を備えているか。_

#### 具体例

テストコードが期待になっていない場合や、テストでのパラメータに考慮漏れがある場合、指摘の対象になります。

* 対象のメソッドがテストされていない
* テストパターンが網羅できていない（パタメータテスト、閾値テストの不足）
* 分岐がパターンが網羅されていない
* 実装上宣言している静的定数値が直接ハードコードされている
* アーキテクトに準じたテストになっていない

### 7. Document（文章）
#### 定義

_ソースコード上に記載されているdoc、コメントが適切な内容であるか。
また、関連するドキュメントは更新されているか。その内容は適切か。_

#### 具体例

ソースコードに関連するコメントだけでなく、プロジェクトで管理している関連ドキュメント（README）も対象になります。

* 関連ドキュメントの更新漏れ（READMEなど）
* docやコメントの内容が不適切、内容が不適切

## 指摘対応の要否

コードレビューでは上記の7つの観点に加えて、指摘修正の要否を4つの累計に分けてコメントをします。

| 観点 | 概要 |
| - | - |
| MUST | PR、MRをマージするためには必ず修正が必要 |
| SHOULD | 修正なしにマージすることはできるが、リリースまでには修正が必要 |
| IMO | レビューアー観点の意見。修正不要 |
| NITS | IMOより細かい意見など。修正不要 |

コードレビューでマージするために必要な修正は MUST 指摘となります。 MUST と SHOULD の使い分けは難しい部分もありますが、SHOULD での指摘事項は別途リリースまでに対応するなど、レビューアーを相談して対応を進めます。一方、IMO や NITS は修正は不要ですが、修正しない場合はその旨をコメントに返信してもらい、コメントを閉じてからマージする運用をしています。

## 具体的な利用方法

実際にコードレビューをするとき、上記の 7 つの観点と修正の温度感をこのように交えた`Prefix`を付けて、コメントをします。

### 指摘例

___<font color="orange">MUST(Design): </font>ドメインロジックがControllerクラスに実装されてます。 domain層の対象packageに新しくクラスを作成して実装を移してください。___

## 最後に

このように、レビューコメントを分析し PDCA サイクルを回すことで、技術的課題へのアプローチ方法も見えてきます。また、コードレビューでオブジェクト指向が学べるといった恩恵を受けることできるので、ぜひチームに合ったコードレビューガイドラインを作成してみてはいかがでしょうか。

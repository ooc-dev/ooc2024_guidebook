= TypeScript の型プログラミング

//flushright{
株式会社スリーシェイク 佐藤慧太@SatohJohn
//}

株式会社スリーシェイク 佐藤慧太@SatohJohnです。 Google Cloud と TypeScript が大好きです。

今回の記事については TypeScript で型プログラミングについてです。

そもそも型の重要性については、いたるところで行われておりますし、文字数の限りもあるので、今回は「重要である」という前提で、
その型をどうやってプログラミングしていくかにフォーカスしてお話したいと思います。

型プログラミングを身につけて、プログラミングにおける Toil をなくしていきましょう。

== TypeScript で型プログラミング

型が重要だからこそ、きちんと書いていく必要があります。とはいえ、複数回同じようなことを書くのは、やはり苦痛です。

例えば、型をプログラミングするモチベーションとして、以下のようなパターンがあります。

 1. object A と object B のパラメータは同じ型にしたい
 2. master データとして、配列を用意してそれを元に入力値を制限したい
 3. object A の key と object B の key に一定の関係性がある

それぞれ詳しく見てみたいと思います。

=== object A と object B のパラメータは同じ型にしたい

アプリケーションを書いていると関係性を持つ型が存在することもあると思います。
例えば以下のようなパターンです。

//emlist[]{
type Person = {
    id: string
}

type PersonDetail = {
    personId: string
}
//}

これは基本的には中間の型を作ることが多いと思います。

//emlist[]{
type PersonId = string

type Person = {
    id: PersonId
}

type PersonDetail = {
    personId: PersonId
}
//}

とはいえ、 @<code>{type} を作るのはなんかなーという場合もあるかと思います。その場合は以下のように @<code>{Person} の key をそのまま参照する方法もあります。

//emlist[]{
type Person = {
    id: string
}

type PersonDetail = {
    personId: Person["id"]
}
//}

どちらかといえば、こちらの場合は明示的に @<code>{Person} のものであることを認識できると思います。

=== master データとして、配列を用意してそれを元に入力値を制限したい

@<code>{select} などで @<code>{input} のパターンを絞っている場合、コード上にもそれを反映したい場合があると思います。
コードでは以下のようなパターンです。

//emlist[]{
const animalKinds = ["cat", "dog", "bird", "fish"]

type AnimalKind = string // 上のパターン以外は入らないようにしたい
//}

この場合、以下のように書き換えることができます。

//emlist[]{
// `as const` とすることで `animalKinds` は ["cat", "dog", "bird", "fish"] という型になります
// そのため as const がなければ kind: string と同義になります
const animalKinds = ["cat", "dog", "bird", "fish"] as const

type AnimalKind = typeof animalKinds[number] // "cat"|"dog"|"bird"|"fish"
//}

これにより select の値を変化させたらそのまま、型も拡張されることになります。

そもそものこの @<code>{as const} の値自体を縛りたいということも出てくると思います。拡張もある程度のパターンを持たせたいということです。
その際には @<code>{satisfies} というキーワードを利用します。今回説明はできませんが、頭の隅に置いておくと良いかと思います。

=== object A の key と object B の key に一定の関係性がある

例えば、 API から帰ってくる Json の key が snake_case の場合、 TypeScript で camelCase に変換しつつマッピングするかと思います

//emlist[]{
type ResponseBody = {
    user_id: string
}

type User = {
    userId: string
}
//}

この例のように API で指針が決まっている場合、変換用の型を書いていくのはとても面倒です。

この課題について OpenAPI など自動生成を使うことで対応している方も多いと思います。

ここではあえて、自作するとした場合にどうするかを考えます。

結果を先にお見せすると、以下のように型をプログラミングすることで対応ができます。

//emlist[]{
// SnakeをCamelにします
type SnakeToCamelize<T extends string> = T extends `${infer A}_${infer B}` ? `${A}${SnakeTo
Camelize<Capitalize<B>>}` : T

// object の key を Snake から Camel にします
type SnakeToCamelizeKeys<T extends object> = {
    [key in keyof T as key extends string ? SnakeToCamelize<key> : key]: T[key] extends object ? 
	SnakeToCamelizeKeys<T[key]> : T[key]
}

type ResponseBody = {
    user_id: string
    detail: {
        name: string
        favorite_food: string
        note: {
            content_id: string
        }
    }
}

type User = SnakeToCamelizeKeys<ResponseBody> 
// {
//     userId: string
//     detail: {
//         name: string
//         favoriteFood: string
//         note: {
//             contentId: string
//         }
//     }
// }
//}

これには　TypeScript の機能の @<code>{Conditional Types}、@<code>{Template Literal Types} と @<code>{Mapped Types} を利用します

上記を、もう少し詳しく見てみます。

//emlist[]{
type SnakeToCamelize<T extends string> = T extends `${infer A}_${infer B}` ? `${A}${SnakeTo
Camelize<Capitalize<B>>}` : T
//}

この処理は

 * @<code>{infer} を使い `_` で T を分割し、前半部分を A 、 後半部分を B とキャプチャします( @<code>{>{}`${infer A}_${infer B}} )
 * B を @<code>{Capitalize} します( @<code>{Capitalize} は先頭文字だけを大文字にする TypeScript にある機能になります )
 * A はそのままで、変換した B を再起し、連結しています(`${A}${SnakeToCamelize<Capita@<br>{}lize<B>>}`)
 * `_` で分けれないのであれば、そのまま返却します

そのため camel_case を snakeCaseにできるということです。
ただし、これは T に入ってきたものが string を継承している必要があります。つまり object ではいけません。

そのため　@<code>{SnakeToCamelizeKeys} を作成します。これを実現できるのが @<code>{Mapped Types} という機能になります。@<code>{Mapped Types} は基本 key の変換ができます。

//emlist[]{
type SnakeToCamelizeKeys<T extends object> = {
    [key in keyof T as key extends string ? SnakeToCamelize<key> : key]: T[key] extends object ?
	 SnakeToCamelizeKeys<T[key]> : T[key]
}
//}

この処理は

 * T の各 key を使って新しい key を作ります( @<code>{key in key of T} )
 * しかし、 key は string であれば camelCase にして反映します( @<code>{as key extends string ? SnakeToCamelize<key>} )
 * key がstringでなければそのまま返却します( @<code>{: key} )
 * T が object であれば、再起して今までの処理を実施、でなければ、型をそのまま利用して mapping します@<br>{}( @<code>{: T[key] extends object ? SnakeToCamelizeKeys<T[key]> : T[key]} )

そのため、各 key がリネームされて新しい型が作られるということになります。

これらを踏まえれば、ある程度好きなような型の変換ができることになります。

//emlist[]{
type Plural<T> = T extends `${infer F}y`
  ? `${F}ies`
  : T extends string
  ? `${T}s`
  : T
//}

== 型プログラミングを使わないほうが良いとき

どんな場合も、型プログラミングをしていったほうが良いのでしょうか？

残念ながら、私には型プログラミングは複雑に見えがちだと思います。
少なくとも私がこれを見て理解するまで数年単位でかかりました。

なので、難しいものは使わない、本当に困っているときに使うとして、そもそも型が簡潔になるような、データ設計を行うのが良い手段だと思います。

== まとめ

サンプルを交えて、型のプログラミングをどうやれば良いのかについて、簡単にではありますが説明をさせていただきました。

型を定義するのは重要ですが、面倒な側面もあります。なので楽をしましょう。とはいえ、あまり型を複雑にしないようにしましょう。
私は、どうすればいいかなと悩んだ場合、普通のプログラミングと同じように YAGNI や DRY などの原則を意識してみると良いと思います。

以上、簡単な型プログラミングについての紹介ではありますが、弊社[株式会社スリーシェイク](https://3-shake.com/) に興味ありましたら、ぜひお声掛けください。

　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/satohjohn.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        佐藤慧太@SatohJohn https://twitter.com/SatohJohn\\
        株式会社スリーシェイク https://3-shake.com/
    \end{minipage}
    \hspace{1ex}
//}
//noindent
Google Cloud と TypeScript が大好きです。
//noindent
趣味は嫁の観察です。そして、社会の Toil をなくせるように日々日々精進しております。
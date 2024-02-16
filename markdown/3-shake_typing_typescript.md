# TypeScript の型プログラミング

株式会社 スリーシェイク 佐藤慧太 (@SatohJohn) です。 Google Cloud と TypeScript が大好きです。

今回の記事については TypeScript で型プログラミングについてです。
そもそも型の重要性については、いたるところで行われておりますし、文字数の限りもあるので、今回は「重要である」という前提で、
その型をどうやってプログラミングしていくかにフォーカスしてお話したいと思います。

## TypeScript で型プログラミング

型をプログラミングするモチベーションとして、普通のプログラミングと全く同じく変数による「共通化」があると思います。

例えば、以下のようなパターンがあると思います。
1. object A と object B のパラメータは同じ型にしたい
2. master データとして、配列を用意してそれを元に入力値を制限したい
3. object A の key と object B の key に一定の関係性がある

それぞれ詳しく見てみたいと思います。

### 1. object A と object B のパラメータは同じ型にしたい

アプリケーションを書いていると関係性を持つ型が存在することもあると思います。
例えば以下のようなパターンです。

```ts
type Person = {
    id: string
}

type PersonDetail = {
    personId: string
}
```

これは基本的には中間の型を作ることが多いと思います。

```ts
type PersonId = string

type Person = {
    id: PersonId
}

type PersonDetail = {
    personId: PersonId
}
```

とはいえ、 `type` を作るのはなんかなーという場合もあるかと思います。その場合は以下のように `Person` の key をそのまま参照する方法もあります。

```ts
type Person = {
    id: string
}

type PersonDetail = {
    personId: Person["id"]
}
```

どちらかといえば、こちらの場合は明示的に `Person` のものであることを認識できると思います。

### 2. master データとして、配列を用意してそれを元に入力値を制限したい

`select` などで `input` のパターンを絞っている場合、コード上にもそれを反映したい場合があると思います。
コードでは以下のようなパターンです。

```ts
const animalKinds = ["cat", "dog", "bird", "fish"]

type AnimalKind = string // 上のパターン以外は入らないようにしたい
```

この場合、以下のように書き換えることができます。

```ts
// `as const` とすることで `animalKinds` は ["cat", "dog", "bird", "fish"] という型になります
// そのため as const がなければ kind: string と同義になります
const animalKinds = ["cat", "dog", "bird", "fish"] as const

type AnimalKind = typeof animalKinds[number] // "cat"|"dog"|"bird"|"fish"
```

これにより select の値を変化させたらそのまま、型も拡張されることになります。

そもそものこの `as const` の値自体を縛りたいということも出てくると思います。拡張もある程度のパターンを持たせたいということです。
その際には `satisfies` というキーワードを利用します。今回説明はできませんが、頭の隅に置いておくと良いかと思います。

### 3. object A の key と object B の key に一定の関係性がある

例えば、 API から帰ってくる Json の key が snake_case の場合、 TypeScript で camelCase に変換しつつマッピングするかと思います

```ts
type ResponseBody = {
    user_id: string
}

type User = {
    userId: string
}
```

この例のように API で指針が決まっている場合、変換用の型を書いていくのはとても面倒です。

この課題について OpenAPI など自動生成を使うことで対応している方も多いと思います。

ここではあえて、自作するとした場合にどうするかを考えます。

結果を先にお見せすると、以下のように型をプログラミングすることで対応ができます。

```ts
// SnakeをCamelにします
type SnakeToCamelize<T extends string> = T extends `${infer A}_${infer B}` ? `${A}${SnakeToCamelize<Capitalize<B>>}` : T

// object の key を Snake から Camel にします
type SnakeToCamelizeKeys<T extends object> = {
    [key in keyof T as key extends string ? SnakeToCamelize<key> : key]: T[key] extends object ? SnakeToCamelizeKeys<T[key]> : T[key]
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
```

これには　TypeScript の機能の `Conditional Types`、`Template Literal Types` と `Mapped Types` を利用します

上記を、もう少し詳しく見てみます。

```ts
type SnakeToCamelize<T extends string> = T extends `${infer A}_${infer B}` ? `${A}${SnakeToCamelize<Capitalize<B>>}` : T
```

この処理は
- `infer` を使い `_` で T を分割し、前半部分を A 、 後半部分を B とキャプチャします( `${infer A}_${infer B}` )
- B を `Capitalize` します( `Capitalize` は先頭文字だけを大文字にする TypeScript にある機能になります )
- A はそのままで、変換した B を再起し、連結しています(`${A}${SnakeToCamelize<Capitalize<B>>}`)
- `_` で分けれないのであれば、そのまま返却します

そのため camel_case を snakeCaseにできるということです。
ただし、これは T に入ってきたものが string を継承している必要があります。つまり object ではいけません。

そのため　`SnakeToCamelizeKeys` を作成します。これを実現できるのが `Mapped Types` という機能になります。`Mapped Types` は基本 key の変換ができます。

```ts
type SnakeToCamelizeKeys<T extends object> = {
    [key in keyof T as key extends string ? SnakeToCamelize<key> : key]: T[key] extends object ? SnakeToCamelizeKeys<T[key]> : T[key]
}
```

この処理は
- T の各 key を使って新しい key を作ります( `key in key of T` )
- しかし、 key は string であれば camelCase にして反映します( `as key extends string ? SnakeToCamelize<key>` )
- key がstringでなければそのまま返却します( `: key` )
- T が object であれば、再起して今までの処理を実施、でなければ、型をそのまま利用して mapping します( `: T[key] extends object ? SnakeToCamelizeKeys<T[key]> : T[key]` )

そのため、各 key がリネームされて新しい型が作られるということになります。

これらを踏まえれば、ある程度好きなような型の変換ができることになります。

```ts
type Plural<T> = T extends `${infer F}y`
  ? `${F}ies`
  : T extends string
  ? `${T}s`
  : T
```

## 型プログラミングを使わないほうが良いとき

どんな場合も、型プログラミングをしていったほうが良いのでしょうか？

残念ながら、私には型プログラミングは複雑に見えがちだと思います。
少なくとも私がこれを見て理解するまで数年単位でかかりました。

なので、難しいものは使わない、本当に困っているときに使うとして、そもそも型が簡潔になるような、データ設計を行うのが良い手段だと思います。

## まとめ

サンプルを交えて、型のプログラミングをどうやれば良いのかについて、簡単にではありますが説明をさせていただきました。
型を書いていくのは大事ですが、面倒な側面もあります。なので楽をしましょう。とはいえ、あまり型を複雑にしないようにしましょう。

型プログラミングも普通のプログラミングと同じように YAGNI や DRY などの原則を意識してみると良いと思います。

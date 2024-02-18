# OOUI を関数で考える

はじめまして、株式会社ゆめみ新卒フロントエンドエンジニアの中川孟です。

突然ですがみなさん、OOUI を実践していらっしゃるでしょうか。デザイナーの皆さんには OOUI の実践者が少なくないと思いますが、エンジニアで OOUI を活用している方はまだあまりいないかもしれません。

私は、OOUI がエンジニアにとっても優れたメンタルモデルを提供すると気付き、日々の業務で OOUI を実践しています。OOUI はオブジェクトという「共通認識」を通じてデザイナーとエンジニアがより良く協業するための優れた手法です。

そして、デザイナーとエンジニアのより良い協業を模索したいという思いから、この半年、同期のデザイナーとともに OOUI 勉強会を継続して開催してきました。「OOUI を関数で考える」というテーマで、デザイナー向けに、React, TypeScript を用いて OOUI をプログラムで記述してみようという内容の勉強会です。

最近、その勉強会の効果を少しずつ実感しています。折角の機会ですので、本稿ではその勉強会の中でも特に肝心な内容を紹介したいと思います。

## UI を関数で考える

UI = f(state) や UI = f(data) などで示される通り、多くのフロントエンドエンジニアは「UI とは関数である」というメンタルモデルを共有しています。

そもそも「関数」が多くのデザイナーにとって慣れないものであるようなのですが、ここでは簡単に「何かを入れたら何かが出るもの」をイメージしてください。お金を入れたらカプセルが出てくるガチャガチャなんかは関数のいい例でしょう。

関数で重要になるのが「引数」と「返り値」という概念です。簡単にいえば、引数は「関数に入れるもの」、返り値は「関数から出てくるもの」です。先の例で言えば、お金が引数、カプセルが返り値にあたります。

これを踏まえると、UI = f(data) は「data を引数にとる関数 f の返り値として UI が存在する」といったような意味になります。UI が関数で表現されるのです。

素朴な例を挙げるとすれば、下記のような Greet 関数コンポーネントはいかがでしょう。name を引数に取って、p タグに囲まれた”Hello, 〇〇”という文字列を返します。name に hajime を入れると、 `<p>Hello, hajime</p>` が返ってくることになります。

```tsx
const Greet = ({ name }) => {
  return <p>Hello, {name}</p>;
};
```

この勉強会のゴールの 1 つは、OOUI を「オブジェクトを受け取って UI を返す関数」として理解すること、すなわち OOUI = f(object) というメンタルモデルを獲得することです。そしてそのために、オブジェクトを引数に取る関数コンポーネントを記述することに挑戦します。

ところで、OOUI の「オブジェクト」とは別にプログラミング言語にも「オブジェクト/object」という機能が存在します。少し紛らわしいので、これ以降「OOUI の文脈でのオブジェクト/object」は「モデル/model」と表現します。

## モデルを TypeScript の型で記述する

OOUI = f(model) をプログラムで表現していく上で、まず重要なのは「それがどのようなモデルなのか」ということです。

フロントエンドの現場では、しばしばそれを TypeScript の「型/type」という機能で表現します。例えば Todo モデルは下記のように記述されます。

```tsx
type Todo = {
  id: string;
  title: string;
  dueTo: Date;
  isDone: boolean;
};
```

Todo がモデル、その中身の title や isDone がプロパティです。title や isDone の注釈に使われている string や boolean などはそのプロパティのデータ型を表現しています。

勉強会では「ある人に関する情報」が詰まった Profile モデルを作成することにしました。「プロフィールとしてどのような情報があったらいいか」というモデリングから始めて、それがどのようなプロパティ・データ型で表現できるかを考えていきます。

実際に、最初に出来上がったモデルがこちらです。名前やアイコン、お気に入り情報が詰まっています。

```tsx
type Profile = {
  name: string;
  iconUrl: string;
  favorits: {
    foods: string[];
    colors: string[];
    artists: string[];
  };
};
```

このモデルに沿ったデータを記述してみると、例えばこのようになります。

```tsx
const profileA: Profile = {
  name: "hajime",
  iconUrl: "https://example.com/hajime.png",
  favorits: {
    foods: ["apple", "banana"],
    colors: ["blue", "green"],
    artists: ["aiko", "spitz"],
  },
};
```

## モデルに関心のある関数を書く

OOUI = f(model) において、モデルは関数の引数に登場します。そして TypeScript を使う上では、関数がどのような引数を受け取るのかもまた、ひとつの型として表現されます。

試しに、ひとつの Profile を表現する ProfileCard コンポーネントを書いてみましょう。

```tsx
interface Props {
  profile: Profile;
}

const ProfileCard: FC<Props> = ({ profile }) => {
  return (
    ...
  );
};
```

これまでよりもぐっと複雑なコードに見えますね。 下の`const ProfileCard` の方は、 `: FC<Props>` の部分を除けば Greet 関数と構造が同じです。 `interface Props` の方は、`type Todo` の書き方と大体同じです。

上記のコードの概要を説明すると、「ProfileCard は引数 profile を受け取る関数コンポーネントです。そして引数 profile は Profile 型です」といった意味になります。

この場ではコードの書き方の話はさておき、少し遠目で見てイメージを喚起することに意識を向けてみてください。

- `interface Props` に続く `{ profile: Profile }`の部分が、コンポーネント記述の中の `FC<Props>`に続く `{ profile }` と対応していること
- `FC<Props>`という ProfileCard コンポーネントに対する型注釈が OOUI = f(model) の右辺のイメージとぴったり重なること（FC が f、Props が model にあたります）

などがわかると、「OOUI を関数で考える」のイメージがなんとなく伝わってきます（はい、この説明だけでは中々わかりません、すみません）。

ProfileCard はモデルの単体（あるいは詳細、シングル）に関心のあるコンポーネントですが、モデルのリストに関心のあるコンポーネントももちろん考えられます。その際、インターフェイスにも違いが出ます。

```tsx
interface Props {
  profiles: Profile[];
}

export const ProfileList: FC<Props> = ({ profiles }) => {
  return (
     ...
  );
};
```

要するにモデルの配列を引数に取るということですが（引数名も複数形にしました）、こう見ると`interface Props`にコンポーネント（関数）の関心が現れることがはっきりわかります。

## モデルの具体としてのデータと UI

さて、関数コンポーネントを定義しただけでは UI が立ち上がりません。UI 表示させたい場所で適当な引数を渡して関数を呼び出す必要があります。ガチャガチャにお金をいれてハンドルを回すのと同じことです。

React では関数コンポーネントの呼び出しは

```tsx
<ProfleCard />
```

のような素朴なタグ表現で完結しますが、ProfleCard には引数が必要なのでした。それを呼び出しのタイミングで渡してあげます。次のようになります。

```tsx
<ProfileCard profile={?}>
```

さて、？に入るのはどのようなデータでしょうか。その説明が先程記述した

```tsx
interface Props {
  profile: Profile;
}
```

にあたります。「引数 profile は Profile 型のデータです」ということです。

この Profile 型のデータの一例として、「モデルを TypeScript の型で記述する」の章で書いた`profileA`が挙げられます。

```tsx
const profileA: Profile = {
  name: "hajime",
  iconUrl: "https://example.com/hajime.png",
  favorits: {
    foods: ["apple", "banana"],
    colors: ["blue", "green"],
    artists: ["aiko", "spitz"],
  },
};
```

ということで、この`profileA`を`ProfileCard`に渡してあげれば、そのデータに沿った UI が表示されることになります。

```tsx
<ProfileCard profile={profileA}>
```

具体的にどのような UI が表示されるんだ？ということについては、実は先程「…」で省略していた ProfileCard 関数の返り値に記述されています。例えば以下のようになりますが、詳細な説明はお近くのフロントエンドエンジニアに聞いてみてください。

```tsx
const ProfileCard: FC<Props> = ({ profile }) => {
  return (
    <div className="max-w-2xl py-8 flex items-center justify-center rounded-2xl border border-gray-300 bg-white">
      <div className="max-w-xl flex flex-col gap-6">
        <h1 className="text-xl text-gray-500">My Profile</h1>

        <div className="flex items-center justify-between">
          <div>
            <h2 className="text-4xl font-bold">{profile.name}</h2>
            <ul>
              <li className="text-base">{profile.role}</li>
              <li className="text-base">{profile.catchcopy}</li>
            </ul>
          </div>
          <img
            className="shadow-xl"
            width={136}
            src={profile.iconUrl}
            alt="プロフィール写真"
          />
        </div>

        <ul className="flex gap-10">
          <li className="w-40 h-auto bg-blue-100 rounded-lg flex flex-col items-start text-sm p-4">
            {profile.favorits.foods.map((food) => (
              <span>{food}です。</span>
            ))}
          </li>
          <li className="w-40 h-auto bg-yellow-100 rounded-lg flex flex-col items-start text-sm p-4">
            {profile.favorits.colors.map((color) => (
              <span>{color}</span>
            ))}
          </li>
          <li className="w-40 h-auto bg-gray-200 rounded-lg flex flex-col items-start text-sm p-4">
            {profile.favorits.artists.map((artist) => (
              <span>{artist}</span>
            ))}
          </li>
        </ul>
      </div>
    </div>
  );
};
```

ちなみに上記は勉強会でデザイナー達が実際に書いたコードですが、彼らは勉強会が始まった当初、HTML/CSS も書けませんでした。ここまで書くのにはたくさんの知識が必要で、改めて彼らに敬意を評します。

## おわりに

いかがだったでしょうか。今回は紙幅の都合で説明をかなり端折っており、デザイナーに向けて書いたものの、プログラミングに慣れない方にとってはいささか伝わりづらい内容になってしまったかもしれません。

エンジニアがデザイナーにとって伝わりづらい説明をしていて、良い協業ができるわけがありません。ですので本稿のわかりづらさはいたく反省するところではありますが、これは決して意図的にわかりづらくしているわけではないのです。両者の間にはコンテキストの広い溝があり、ただ簡単には飛び越えられないということなのです。これは一朝一夕で埋められるものではなく、たゆまぬ歩み寄りが必要になります。そして OOUI は、デザイナーとエンジニアの間で素敵な架け橋となってくれるのです。

一緒に勉強会を作ってくれたデザイナー 2 人がその経験を素敵な note に残しているので、そちらもぜひ御覧ください。

[UI デザイナー 1 年生がコードを書いてみた！-コードから GUI を見てみる-｜こばやし　めいか](https://note.com/meika_1123/n/nc61184d6588f)

[デザイナーとエンジニアのタンゴ 💃 ｜ osamu](https://note.com/onaka_pocopoco/n/nfb6ee80f12d7)

たった 3 人で始めた勉強会ですが、徐々に和を広げていけたらなと考えています。OOUI を用いたデザイナーとエンジニアの協業に興味のある皆さん、ぜひお声がけください。

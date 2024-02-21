= Clean Architecture 入門

//flushright{
	Algomatic ごー @53able
//}

//note{
当稿は、Algomatic 取締役 CTO の南里(なんり)による Getting Started for Clean Architecture の内容を元に、Object-Oriented Conference 2024 ガイドブック向けに加筆・修正したものとなります。

https://github.com/neonankiti/CleanArchitectureSample/wiki/Getting-Started-for-Clean-Architecture
//}

== はじめに

株式会社 Algomatic ソフトウェアエンジニア ごー (@53able) です。

この文章は、Clean Architecture の概念を理解し、実際のプロジェクトでの適用方法を学ぶことを目的としています。特に、React.js と TypeScript を使用した Web アプリケーション開発において、Clean Architecture を適用する方法に焦点を当てます。

== Clean Architecture の必要性

Clean Architecture を採用する前に、それが解決しようとしている課題に焦点を当てましょう。主に2つの大きな課題があります。

=== 大規模・多人数によるソフトウェア開発の効率化

大規模なチームでの Web システム開発においては、多くの開発者が協力してアプリケーションを構築します。この場合、開発者それぞれが異なるスキルセットを持っているため、アーキテクチャ、フレームワーク、プログラミング言語に対する意見の相違や衝突が生じることがあります。Clean Architecture は、このような問題を解決するために活用されます。開発者が効率的に協力できるよう、一定の自由度を保ちつつ、守るべきソフトウェアの原則を遵守させます。

=== ビジネス要求の変化に伴うシステム要件の変更への対応

Webアプリケーションの開発フローは一般的に以下のように進みます。

 * ビジネス要求
 * システム要件定義
 * 設計・実装
 * テスト
 * リリース

これらのプロセスは互いに依存関係にあります。例えば、システム要件はビジネス要求に依存し、設計・実装はシステム要件に依存します。

プロダクトのコンセプト実証(Proof of Concept, PoC)段階では、ビジネス要求の変更が頻繁に起こりがちで、これが全ての下位プロセスに影響を及ぼします。ビジネス要求やシステム要件が不明確であるほど、その影響は大きくなり、持続可能なソフトウェア開発が困難になります。

Clean Architecture を利用するもう一つの理由は、設計・実装段階において、上位のプロセスからの影響を最小化できる点にあります。

== Clean Architecture とは

Clean Architecture の最も重要な特徴は、 @<b>{依存関係が外部から内部へと制限されている}ことです。これにより、ソフトウェアとしての低結合度と高凝集度を達成することが可能になります。

//image[CleanArchitecture][Clean Architectureの概念図@<fn>{architecture}][scale=0.6]

//footnote[architecture][https://blog.cleancoder.com/uncle-bob/images/2012-08-13-the-clean-architecture/CleanArchitecture.jpg より引用]

== 前提知識

Clean Architecture における分割手法は、レイヤー（UI、ドメイン、データなどのレイヤー）に適用されます。Clean Architecture の適用はレイヤードアーキテクチャに限定されませんが、TypeScript を使用したWebアプリケーション開発で高い汎用性を持つため、レイヤーに適用されます。

=== SOLID 原則の復習

Clean Architecture の実装に取り組む前に、SOLID原則の復習が有用です。kgmyshinさんの @<b>{solid+cqs+dry}@<fn>{scd}の資料が参考になります。特に、依存関係逆転の原則（Dependency Inversion Principle, DIP）はClean Architecture でよく誤解される部分です。レイヤー分割では、ドメインとデータの間の依存関係に DIP が用いられます。


//footnote[scd][solid+cqs+dry (https://speakerdeck.com/kgmyshin/solid-plus-cqs-plus-dry)]

これらの基礎が理解できたら、次に「The Clean Architecture」@<fn>{theCleanArchitecture}のドキュメントを読んでみましょう。
//footnote[theCleanArchitecture][The Clean Architecture(https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html)]

=== レイヤードアーキテクチャにおける依存関係とクラス

レイヤードアーキテクチャでは、レイヤー間の依存関係と、各レイヤーに属するクラスに特に注意を払う必要があります。モジュールを UI（components）、ドメイン、データに分割します。ドメインが最も内側に位置するため、依存関係は以下のようになります。

 * components → domain
 * data → domain

== Next.js プロジェクトでの Clean Architecture の適用

Clean Architecture の概念を理解したら、実際のプロジェクトでの適用方法を見ていきましょう。

各モジュールに属するクラスは以下のように配置されます。
今回は、Next.js プロジェクトを例に説明します。

//emlist[]{
.
├── public
│   └── ...
├── src
│   ├── components
│   │   ├── SampleComponent.tsx
│   │   └── ...
│   ├── domain
│   │   ├── SampleUseCase.ts
│   │   ├── SampleRepository.ts
│   │   └── ...
│   ├── data
│   │   ├── SampleRepositoryImpl.ts
│   │   ├── SampleDataStore.ts
│   │   └── ...
│   ├── viewModels
│   │   ├── SampleViewModel.ts
│   │   └── ...
│   ├── pages
│   │   ├── index.tsx
│   │   └── ...
│   └── ...
├── styles
│   └── ...
├── .next
├── node_modules
├── package.json
└── ...
//}

実装を進めることで、これらの概念への理解を深めましょう。

== 実装の流れ

実際の手順とは少し違いますが、初心者でも始めやすいと感じるおすすめの手順は以下です。 用語に関しては、独自解釈でつけている部分がありますが、各ステップで説明します。 「[お前らがModelと呼ぶアレをなんと呼ぶべきか。近辺の用語(EntityとかVOとかDTOとか)について整理しつつ考える](https://qiita.com/takasek/items/70ab5a61756ee620aee6)」 が参考になります
> https://qiita.com/takasek/items/70ab5a61756ee620aee6

 1. DTO (Data Transfer Object) の作成
 2. ドメインオブジェクトの作成
 3. リポジトリの作成
 4. DataStore/RepositoryImpl の作成
 5. UseCase の作成
 6. ViewModel の作成
 7. コンポーネントの作成

=== DTO (Data Transfer Object) の作成

DTO は、外部 API やデータベースから取得したデータをアプリケーション内で扱いやすい形に変換するためのオブジェクトです。これにより、API やデータベースのスキーマがアプリケーションの内部表現に直接影響を与えることを防ぎます。

例えば、ユーザー情報を取得するAPIから返される JSON データを表す `UserDTO` を TypeScript で以下のように定義することができます。

//emlist[]{
interface UserDTO {
  id: string;
  username: string;
  email: string;
}
//}

=== ドメインオブジェクトの作成

ドメインオブジェクトは、アプリケーションのビジネスロジックやルールをカプセル化します。DTO とは異なり、ドメインオブジェクトはアプリケーションのコアな部分を形成し、ビジネスロジックを実装します。

上記の @<code>{UserDTO} を基に、ドメインオブジェクト @<code>{User} を以下のように作成できます。

//emlist[]{
class User {
  constructor(
    public id: string,
    public username: string,
    public email: string
  ) {}

  // ユーザー関連のビジネスロジックをメソッドとして追加
  changeEmail(newEmail: string) {
    this.email = newEmail;
    // メールアドレス変更時のビジネスロジックを実装
  }
}
//}

==== 腐敗防止層（ACL）の必要性

腐敗防止層（ACL）は、異なるシステムやドメイン間の統合点でドメインモデルを外部の不適切な影響から保護する翻訳層として機能します。 外部 API からのデータをドメインオブジェクトに適応する際に ACL を使用することで、モデルの整合性を維持し、システム間の結合度を低減させ、変更に対して強固な設計を実現します。 TypeScript での ACL 実装では、インターフェース、アダプター、ファサードなどのデザインパターンを用いて、外部データを内部モデルに適合させる変換やフィルタリングを行います。

例えば、外部 API から取得したユーザー情報をドメインオブジェクトに変換する際に、ACL を使用して変換処理を行うことができます。外部 API からは以下のような形式の JSON データが返されるとします。

//emlist[]{
{
  "user_id": "123",
  "first_name": "John",
  "last_name": "Doe",
  "email": "john.doe@example.com"
}
//}

このデータを @<code>{User} クラスのインスタンスに変換するために、以下のようなアダプターを作成します。

//emlist[]{
interface ExternalUser {
  user_id: string;
  first_name: string;
  last_name: string;
  email: string;
}

class UserAdapter {
  static fromExternal(externalUser: ExternalUser): User {
    return new User(
      externalUser.user_id,
      `${externalUser.first_name} ${externalUser.last_name}`,
      externalUser.email
    );
  }
}
//}

@<code>{UserAdapter} クラスは、外部データを内部モデルに変換するためのアダプターとして機能します。このように、ACL を使用することで、外部データの変換処理をドメインオブジェクトに隔離し、内部モデルの整合性を維持することができます。

=== リポジトリの作成

リポジトリは、ドメインオブジェクトのコレクションに対するアクセスを抽象化するものです。データの取得や保存などの操作を、具体的なデータソースの詳細（例：データベースや外部 API の呼び出し）から分離します。

@<code>{UserRepository}インターフェースを以下のように定義します。

//emlist[]{
interface UserRepository {
  findById(id: string): Promise<User>;
  save(user: User): Promise<void>;
}
//}

=== DataStore/RepositoryImpl の作成

DataStore は、外部データソース（例：API、データベース）との通信を担当します。RepositoryImpl は、リポジトリの具体的な実装を提供します。

//emlist[]{
// DataStore
class UserApiDataStore {
  async fetchUserById(id: string): Promise<UserDTO> {
    const response = await fetch(`https://example.com/users/${id}`);
    const userDTO: UserDTO = await response.json();
    return userDTO;
  }

  async postUser(userDTO: UserDTO): Promise<void> {
    await fetch(`https://example.com/users/${userDTO.id}`, {
      method: 'POST',
      body: JSON.stringify(userDTO),
      headers: {
        'Content-Type': 'application/json'
      }
    });
  }
}

// RepositoryImpl
class UserRepositoryImpl implements UserRepository {
  private dataStore: UserApiDataStore;

  constructor(dataStore: UserApiDataStore) {
    this.dataStore = dataStore;
  }

  async findById(id: string): Promise<User> {
    const userDTO = await this.dataStore.fetchUserById(id);
    return new User(userDTO.id, userDTO.username, userDTO.email); // DTOからドメインオブジェクトへの変換
  }

  async save(user: User): Promise<void> {
    const userDTO: UserDTO = { id: user.id, username: user.username, email: user.email }; // ドメインオブジェクトからDTOへの変換
    await this.dataStore.postUser(userDTO);
  }
}
//}

=== UseCase の作成

ユースケースは、ユーザーの意図に応じたアプリケーションのビジネスロジックを実装します。例えば、ユーザーがメールアドレスを更新する操作は `UpdateUserEmailUseCase` として定義できます。

//emlist[]{
class UpdateUserEmailUseCase {
  constructor(private userRepository: UserRepository) {}

  async execute(userId: string, newEmail: string): Promise<void> {
    const user = await this.userRepository.findById(userId);
    user.changeEmail(newEmail);
    await this.userRepository.save(user);
  }
}
//}

=== ViewModel の作成

ViewModel は、ビュー（UI）に表示するデータとロジックを保持します。React では、カスタムフックとして ViewModel を実装することが一般的です。ViewModel は、ユースケースを使用してドメインロジックを実行し、その結果を UI 層に反映します。

//emlist[]{
function useUserViewModel(userUseCase: UpdateUserEmailUseCase) {
  const [user, setUser] = useState<User | null>(null);

  const updateUserEmail = async (userId: string, newEmail: string) => {
    await userUseCase.execute(userId, newEmail);
    // ユーザー情報の更新処理など
  };

  return {
    user,
    updateUserEmail
  };
}
//}

=== コンポーネントの作成

最後に、ViewModel を使用して UI コンポーネントを作成します。

//emlist[]{
function UserComponent() {
  const userUseCase = new UpdateUserEmailUseCase(new UserRepositoryImpl(new UserApiDataStore()));
  const { user, updateUserEmail } = useUserViewModel(userUseCase);

  return (
    <div>
      <h1>User Information</h1>
      <p>Username: {user?.username}</p>
      <p>Email: {user?.email}</p>
      <button onClick={() => updateUserEmail(user?.id, '')}>Update Email</button>
    </div>
  );
}
//}

これらのステップを通じて、Clean Architecture を実装することで、アプリケーションの各層を明確に分離し、保守性や拡張性の高い設計を実現します。

== まとめ

Clean Architecture の原則をフロントエンド開発、特に React.js のコンテキストに適用することで、ビジネスロジックと UI の分離を達成し、アプリケーションの保守性や拡張性を高めることが可能です。このアプローチでは、ドメインオブジェクト、リポジトリ、ユースケース、ViewModel が核となります。重要なのは、Clean Architecture を厳格なルールではなく、ガイドラインとして捉え、プロジェクトの具体的な要件や状況に応じて柔軟に適応させることです。目的はコードの整理と理解のしやすさ、変更の容易さを確保することにあります。

=== 参考文献

 * Getting Started for Clean Architecture: https://github.com/neonankiti/CleanArchitectureSample/wiki/Getting-Started-for-Clean-Architecture
 * The Clean Architecture: https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html
 * solid+cqs+dry: https://speakerdeck.com/kgmyshin/solid-plus-cqs-plus-dry
 * お前らがModelと呼ぶアレをなんと呼ぶべきか。近辺の用語(EntityとかVOとかDTOとか)について整理しつつ考える: https://qiita.com/takasek/items/70ab5a61756ee620aee6


　


　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/53able.jpg}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        ごー　@53able https://x.com/53able\\
		株式会社 Algomatic
    \end{minipage}
    \hspace{1ex}
//}
株式会社 Algomatic でシゴラク AI のソフトウェアエンジニアとして、生成 AI 産業の一丁目一番地を目指しています。
ランニング、自転車、バイク、キャンプを趣味としています。

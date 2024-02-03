= PHP製OSSに見るデザインパターンの具体例3選+α

//flushright{
hirokinoue
//}

オブジェクト指向、デザインパターン…概念はなんとなくわかった気がするんだけど、どのように使えばいいのだろうか…？そんな感覚を持ったことが誰しも一度はあるのではないでしょうか？そんな時、具体例を参照することで理解を深められると考えています。そこで、PHP製のOSSに見られる生きた事例3つ+αを紹介したいと思います。

ここで紹介するデザインパターンの実装は、書籍やネットで紹介されているものと完全一致するとは限りませんが、それぞれのパターンが有するべき特徴を持っていると考えています。紙面の都合で、説明に関係のある部分のみを抜粋します。コードの全量はGitHubを参照して下さい。

== 1. ストラテジパターン@PHPUnit

以下の特徴を備えたストラテジパターンの一種と捉えて紹介します。

 * 同種の振る舞い（同じシグネチャ）を持ち、詳細の異なる実装がいくつかある。
 * 実行時のコンテクストに応じてその実装が選択される。

PHPUnitはユニットテストのライブラリです。以下のようにしてテストを記述します。この後assertEqualsメソッド@<fn>{assertEquals}を深掘ってゆきます。

//footnote[assertEquals][https://docs.phpunit.de/en/9.6/assertions.html#assertequals ]

//blankline

//emlist[クライアントコード例][php]{
final class EqualsTest extends TestCase
{
	public function testFailure(): void
	{
		// $this->assertEquals($expected, $actual);
		$this->assertEquals(1, 0);
	}
}
//}

ユニットテストにおいて期待値と実行結果が一致することを確認するassertEqualsメソッドは内部でIsEqualクラスを使用しています。ここでストラテジパターンが活用されています。初めにクラスの関係を確認します。

//embed{
    \begin{center}
        \includegraphics[width=0.7\linewidth]{images/chap-guest-hirokinoue/strategy.png}
    \end{center}
//}

それでは見てゆきます。IsEqualと、そこで使用されるComparatorに注目します。Comparatorには実装がいくつかあります。IsEqualは実行時のコンテクスト（テストにおける期待値と実行結果の組み合わせ）に応じて実装を選択します。Comparatorからのぞいてみます。

//blankline

//emlist[Comparator@<fn>{Comparator}][php]{
abstract public function accepts(/* 略 */): bool;
abstract public function assertEquals(/* 略 */): void;
//}

//footnote[Comparator][https://github.com/sebastianbergmann/comparator/blob/146dc7bb46c39d8d42bb0fd8cfdec588f0814f66/src/Comparator.php ]

ComparatorはacceptsメソッドやassertEqualsを持つ抽象クラスで、その実装は様々あります。例えば配列を比較するArrayComparator、数値を比較するNumericComparatorなどです。続いてIsEqualのevaluateメソッドを見てみます。

//blankline

//emlist[IsEqual@<fn>{IsEqual}][php]{
public function evaluate(mixed $other, string $description = '', bool $returnResult = false): ?bool
{
	// 略

	// ComparatorFactoryはFactory（下記）のエイリアス
	// Comparatorの実装の配列を生成する
	$comparatorFactory = ComparatorFactory::getInstance();

	try {
		// Comparatorの配列から実装を選択する
		$comparator = $comparatorFactory->getComparatorFor(
			$this->value,
			$other,
		);

		// 選択した実装に応じたassertEqualsメソッドが実行され
		// 期待値と実行結果が比較される
		$comparator->assertEquals(
			$this->value,
			$other,
		);
//}

//footnote[IsEqual][https://github.com/sebastianbergmann/phpunit/blob/57e07eef9de8e016be8da0f53a35d5df1fa72ebd/src/Framework/Constraint/Equality/IsEqual.php ]

//emlist[Factory@<fn>{Factory}][php]{
// ComparatorFactory::getInstance()からここに辿り着く
private function registerDefaultComparators(): void
{
	// 様々なComparatorの実装を配列に詰めている
	$this->registerDefaultComparator(new MockObjectComparator);
	$this->registerDefaultComparator(new DateTimeComparator);
	// 略

public function getComparatorFor(mixed $expected, mixed $actual): Comparator
{
	// $comparatorはComparator型
	// コンテクストに応じて実装を選択する
	foreach ($this->defaultComparators as $comparator) {
		if ($comparator->accepts($expected, $actual)) {
			return $comparator;
		}
	}
//}

//footnote[Factory][https://github.com/sebastianbergmann/comparator/blob/146dc7bb46c39d8d42bb0fd8cfdec588f0814f66/src/Factory.php ]

== 2. オブザーバパターン@Laravel

以下の特徴を備えたオブザーバパターンの一種と捉えて紹介します。

 * サブジェクト（観察される者）とオブザーバ（観察する者）がいる。
 * サブジェクトはオブザーバを保持する（サブジェクトにオブザーバを登録/解除できる）。
 * サブジェクトの状態変化（サブジェクトからの通知）を察知して、オブザーバの処理を行う。

LaravelはWebアプリケーションフレームワークです。Laravelにおいて、イベントの生成・発行からイベントリスナの処理を行う一連の流れがオブザーバパターンを形成しています。

フレームワークが提供するDispatcherはサブジェクトの役割を果たし、イベントリスナがオブザーバの役割を果たします。Webアプリケーションはこの仕組みを利用するクライアントになります。サブジェクトはオブザーバを保持しますのでDispatcherにイベントリスナを登録する必要があります。また、イベントの種類に応じて通知を受けるべきリスナが異なりますので、イベントとリスナの関連づけも行います。

まとめると、クライアントがイベントやサブジェクトたるDispatcherを生成してオブザーバたるイベントリスナに通知させ、イベントリスナはこれを受けて処理を行います。この仕組みにより、例えば、ユーザー登録すると”ユーザー登録済みイベント”を発行し、これを受けてメールを送信するといった振る舞いを実現できます。

//blankline

//emlist[events/Dispatcher@<fn>{Dispatcher}][php]{
// イベントとイベントリスナを登録する
public function listen($events, $listener = null) {}

// イベントを発行する
public function dispatch($event, $payload = [], $halt = false)
{
	// 略
	
	if ($isEventObject &&
		$payload[0] instanceof ShouldDispatchAfterCommit &&
		! is_null($transactions = $this->resolveTransactionManager())) {
		$transactions->addCallback(
			// リスナを呼び出す
			fn () => $this->invokeListeners($event, $payload, $halt)
		);
		
		return null;
	}

	// リスナを呼び出す
	return $this->invokeListeners($event, $payload, $halt);
}

protected function invokeListeners($event, $payload, $halt = false)
{
	// 略

	foreach ($this->getListeners($event) as $listener) {
		// リスナのhandle()が実行される
		$response = $listener($event, $payload);
//}

//footnote[Dispatcher][https://github.com/illuminate/events/blob/22d6718c5859f2290a8a8ac32b24ed7a4bc1a93f/Dispatcher.php ]

//emlist[リスナ@<fn>{listener}][php]{
class FooListener
{
	public function __construct() {}

	// DispatcherのinvokeListeners()からここに辿り着く
	public function handle(FooEvent $event): void
	{
		// サブジェクトからの通知（イベント）を受けて行う処理
	}
}
//}

//footnote[listener][https://laravel.com/docs/10.x/events#defining-listeners ]

※Laravelでは様々なやり方でイベントとリスナの実装が可能ですがほんの一部だけ紹介しました。

クラスの関係は以下のようになっています。

//embed{
    \begin{center}
        \includegraphics[width=0.45\linewidth]{images/chap-guest-hirokinoue/observer.png}
    \end{center}
//}

== 3. ビジターパターン@PHP-Parser

以下の特徴を備えたビジターパターンの一種と捉えて紹介します。

 * 構造と操作を分離している。
 * 操作の具象を定義することで、構造に対する操作を追加できる。また逆に操作を除くこともできる。

観察対象であるPHP-Parserの説明から始めます。PHP-ParserはPHPの静的解析に利用されるライブラリです。PHPのコードを読み込んでノードと呼ばれるオブジェクトの連なりに変換します。これをASTと呼びますがここでは説明を割愛します。例えば、このコードと下記のASTが対応づきます。下図の四角形がノードです。

//blankline

//emlist[解析対象コードサンプル][php]{
<?php
function foo() {
    echo 'bar';
}
//}

//embed{
	AST
     \begin{flushleft}
       \includegraphics[width=0.6\linewidth]{images/chap-guest-hirokinoue/ast.png}
    \end{flushleft}
//}

PHP-ParserにおいてTraverserとVisitorによりビジターパターンが成立しています。Traverserが構造側の処理を担い、Visitorが操作側の処理を担います。

Traverserは一つ一つノードを読み込みます。また、TraverserはVisitorを保持しており、Visitorにノードを処理させます。つまりすべてのノードはTraverserにより辿られ、Vistitorにより処理されます。Visitorには様々な実装があります。例えば、条件を満たすノードを見つけるVisitorやノードの名前を特定するVisitorがあります。構造と操作を分離するというのは、ノードを辿るという処理とノードに対して行う処理を別々に構築できるということです。

まずクラスの関係を確認します。

//blankline

//embed{
    \begin{center}
        \includegraphics[width=1\linewidth]{images/chap-guest-hirokinoue/visitor.png}
    \end{center}
//}

それでは実装を見てみます。

//blankline

//emlist[NodeTraverser@<fn>{NodeTraverser}][php]{
class NodeTraverser implements NodeTraverserInterface {
	public function __construct(NodeVisitor ...$visitors) {
		// TraverserはNodeVistorを保持している
		$this->visitors = $visitors;
	}

	public function traverse(array $nodes): array {
		// 略
		
		foreach ($this->visitors as $visitor) {
			// TraverserはNodeVistorを使用する
			if (null !== $return = $visitor->beforeTraverse($nodes)) {
				$nodes = $return;
			}
		}
		// 略
			
	protected function traverseNode(Node $node): void {
				// 略

				foreach ($this->visitors as $visitorIndex => $visitor) {
					// TraverserはNodeVistorを使用する
					$return = $visitor->enterNode($subNode);
//}

//footnote[NodeTraverser][https://github.com/nikic/PHP-Parser/blob/ce019e9ad711e31ee87c2c4c72e538b5240970c3/lib/PhpParser/NodeTraverser.php ]

NodeVisitorのシグネチャは以下の通りです。

//blankline

//emlist[NodeVisitor@<fn>{NodeVisitor}][php]{
interface NodeVisitor {
	public function beforeTraverse(array $nodes);
	public function enterNode(Node $node);
	public function leaveNode(Node $node);
	public function afterTraverse(array $nodes);
}
//}

//footnote[NodeVisitor][https://github.com/nikic/PHP-Parser/blob/ce019e9ad711e31ee87c2c4c72e538b5240970c3/lib/PhpParser/NodeVisitor.php ]

NodeVisitorにはFindingVisitorやNameResolverなど様々な実装があります。Visitorの具象クラスはNodeVisitorを実装する抽象クラスであるNodeVisitorAbstractを拡張して作ります。FindingVisitorを見てみます。beforeTraverseやenterNodeを実装していることがわかります。NameResolverも見ていただくと、これと異なる実装を確認していただけます。

//blankline

//emlist[FindingVisitor@<fn>{FindingVisitor}][php]{
class FindingVisitor extends NodeVisitorAbstract {
	public function beforeTraverse(array $nodes): ?array {
		$this->foundNodes = [];
		
		return null;
	}

	public function enterNode(Node $node) {
		$filterCallback = $this->filterCallback;
		if ($filterCallback($node)) {
			$this->foundNodes[] = $node;
		}
		
		return null;
	}
}
//}

//footnote[FindingVisitor][https://github.com/nikic/PHP-Parser/blob/ce019e9ad711e31ee87c2c4c72e538b5240970c3/lib/PhpParser/NodeVisitor/FindingVisitor.php ]

NodeTraverserは具象ではなくNodeVisitorに依存するので、NodeVisitorを実装しているあらゆる具象を利用できます。ノードに対して行いたい処理を個別の具象クラスとして定義し、Traverserに渡すことで処理を追加できるということです。不要な処理はTraverserから取り除くこともできます。

ASTに限らず、順々に読み込んだものに対して処理を行うケース、そしてその処理を着脱したいケースでビジターパターンが適用できます。

== α. インタフェース、多態性の活用

オブジェクト指向を学ぶ過程において、インタフェースの使い道というのはひっかかるポイントの一つではないでしょうか。上述のデザインパターンの中に示唆があります。

ビジターパターンのNodeTraverserを見て下さい。NodeTraverserはVisitorの具象ではなく抽象（NodeVisitor）に依存しています。beforeTraverse(), enterNode(), leaveNode(), afterTraverse()があれば処理を行えるようになっており、NodeVisitorのシグネチャを持つその実装は要件を満たします。これにより、ノードに対して行いたいことに応じて処理を実装し、用途に応じて処理を着脱できます。

ストラテジパターンのFactoryクラス（この事例ではインタフェースではなく抽象クラスへの依存ですが）にも同じような特長が見られます。これらの例の中で、仮にNodeTraverserやComparatorの具象に依存しているとすると、実装が差し替えられず柔軟性を失います。また依存を明示しない（型を示さない）と改修する際にバグを混入させやすくなります。

== おわりに
以上、デザインパターン、インタフェースの使い道について実例を見てきました。あなたが見つけたデザインパターン、オブジェクト指向設計テクニックの生きた事例を紹介していただけるとうれしいです。


　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/hirokinoue.jpg}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        hirokinoue
		https://github.com/hirokinoue
    \end{minipage}
    \hspace{1ex}
//}
株式会社ホワイトプラスでEMをしています。かつてはCOBOLを少々、現在はPHPを少々。静的解析と仲良くなりたい。
= SOLID原則をギュッとまとめた

//flushright{
シキム@sikimuOji
//}

== ギュッとまとめました。

SOLID原則を理解してもらうために、誰しも必ず通るHelloWorldにまとめてみました。

//list[Code1][Code1:HelloWorld]{
// メイン(実行クラス)
public class Main {
    public static void main(String[] args) {
        Printer printer = new SystemPrinter();
        printer.print(new Hello(), new World());
    }
}

// 出力情報
interface FirstData { String create(); }
interface SecondData { String create(); }
class Hello implements FirstData {
    public String create(){
        return "Hello";
    }
}
class World implements SecondData {
    public String create(){
        return "World";
    }
}

// 出力処理
abstract class Printer {
    abstract void print(FirstData first, SecondData second);
}
class SystemPrinter extends Printer{
    public void print(FirstData first, SecondData second){
        System.out.println(first.create() + " " + second.create());
    }
}
//}

//pagebreak

== 解説
それぞれの、クラスに埋め込まれたSOLID原則について解説していきます。

==== 開放閉鎖の原則
ここを変更するだけで、出力方法や出力情報を簡単に変更することができます。
//list[Code2][Code2:開放閉鎖の原則]{
// メイン
public class Main {
    public static void main(String[] args) {
        Printer printer = new SystemPrinter();
        printer.print(new Hello(), new World());
    }
}
//}

==== インターフェース分離の原則
FirstSecondDataというように統合することもできますが、"Hello Japan"も表示するようにしたいときなどに無駄な実装が発生してしまいます。
変更する最小の単位で分けてあるほうが、柔軟な組み合わせで利用することができます。
//list[Code3][Code3:インターフェース分離の原則]{
// 出力情報
interface FirstData { String create(); }
interface SecondData { String create(); }
class Hello implements FirstData {
    public String create(){
        return "Hello";
    }
}
class World implements SecondData {
    public String create(){
        return "World";
    }
}
//}


==== 単一責任の原則
１つのクラスは、１つの変更で同時に行う処理の集まりとしてまとめましょう。複数のタイミングで変更するものが混じっていると、
いずれかの処理を変更したときにどちらも確認が必要となってしまいます。

==== 依存性逆転の原則
変更することの少ない出力処理が、変更の多い出力情報の実装を気にしなくていいように、出力情報のインターフェースを利用しましょう。  

==== リスコフの置換原則
Printerで決めたルール以外のことを実装してしまうと、変更の際に実装を確認する必要が出てきてしまい、バグの要因になるのでやらないようにしましょう。
//list[Code4][リスコフの置換原則]{
// 出力処理
abstract class Printer {
    abstract void print(FirstData first, SecondData second);
}
class SystemPrinter extends Printer{
    public void print(FirstData first, SecondData second){
        System.out.println(first.create() + " " + second.create());
    }
}
//}

== 人間はミスをする生き物
人間はすぐミスをします。特に変更は、変更した場所以外の影響も考える必要があるためミスをしやすいです。しかし、システム開発は、変更の連続です。コーディング中の気付きや、考慮漏れの発覚、ちゃぶ台返しなど大小様々な変更をしなければいけません。 そのため、人間のミスを減らし変更をしやすいコードにすることは、非常に重要になってきます。
SOLID原則を意識して作ることで、変更を最小限にしミスを減らすことができると思います。

==== 今回は、ここまでとさせていただきます。またいつかどこかで(´・ω・`)

　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/sikimu.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        シキム　@sikimuOji https://twitter.com/sikimuOji\\
		所属とか(任意)
    \end{minipage}
    \hspace{1ex}
//}
ひとこと

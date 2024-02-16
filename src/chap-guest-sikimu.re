= SOLID原則をギュッとまとめた

//flushright{
シキム@sikimuOji
//}

== 大事なことは、Hello Worldが教えてくれる。

オブジェクト指向と言えばSOLID原則。Hello Worldにギュッとまとめてみました。

//emlist[]{
// メイン
public class Main {
    public static void main(String[] args) {
        Printer printer = new SystemPrinter();
        printer.print(new Hello(), new World());// 開放閉鎖の原則
    }
}

// 出力処理(単一責任の原則)
abstract class Printer {
    abstract void print(First first, Second second);// 依存性逆転の原則
}
class SystemPrinter extends Printer{
    public void print(First first, Second second){        
        System.out.println(first.create() + " " + second.create());// リスコフの置換原則
    }
}

// 前半出力情報(単一責任の原則)
interface First { String create(); }// インターフェース分離の原則
class Hello implements First {
    public String create(){
        return "Hello";
    }
}

// 後半出力情報(単一責任の原則)
interface Second { String create(); }// インターフェース分離の原則
class World implements Second {
    public String create(){
        return "World";
    }
}
class Japan implements Second {
    public String create(){
        return "Japan";
    }
}
//}

== 解説

==== 単一責任の原則
最小の仕様変更をしたときに、同時に変更するものをクラスにまとめましょう。

==== 開放閉鎖の原則
インターフェースを利用して、呼び出し側の変更だけで済むようにしましょう。

==== 依存性逆転の原則
変更が少ない処理は、変更の多い処理をインタフェースで利用するようにしましょう。

==== リスコフの置換原則
変更による意図しない影響を防ぐため、インターフェースで決めた仕様のみ実装しましょう。

==== インターフェース分離の原則
変更したい最小の単位のインターフェースにまとめましょう。

== おわりに
システム開発において、変更とはとても厄介なものです。
SOLID原則を意識してコードを書くことで、変更や変更による影響を最小限にすることができます。
少しの変更でも修正箇所が肥大化したり不具合が発生するようなときには、
原則に違反していないか見直してみるといいかもしれません。

//blankline

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/sikimu.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        シキム　@sikimuOji https://twitter.com/sikimuOji\\
    \end{minipage}
    \hspace{1ex}
//}
ちょっとJava出来る静岡のおじさんです(´・ω・`)

= SOLID原則をギュッとまとめた

//flushright{
シキム@sikimuOji
//}

== ギュッとまとめました。

SOLID原則を理解してもらうために、誰しも必ず通るHello Worldにまとめてみました。

//blankline

//emlist[ギュッとHello World]{
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
HelloWorldに潜むSOLID原則について解説していきます。

==== 単一責任の原則
最小の仕様変更をしたときに、同時に変更するものをクラスにまとめましょう。
出力方法を変更するならSystemPrinter、出力情報を変更するならHelloやWorldだけになります。

==== 開放閉鎖の原則
インターフェースを利用して、呼び出し側の変更だけで済むようにしましょう。
WorldをJapanに変更するだけで、出力情報を変更することができます。

==== 依存性逆転の原則
変更することが少ない処理は、変更の多い処理をインタフェースで利用するようにしましょう。
FirstやSecondを使うことで、変更の少ない出力処理が変更の多い出力処理に影響しなくなります。

==== リスコフの置換原則
インターフェースで決めた仕様だけを実装しましょう。
SystemPrinterで追加処理を入れてしまうと、別の場所で使う場合に追加処理が意図せず動いたり、
別のPrinterに変更した場合に追加処理が動かなくなってしまうなどしてバグの原因になります。

==== インターフェース分離の原則
インターフェースは、変更したい最小の単位のまとまりにしましょう。
FirstSecondというように統合すると、"Hello World"から"Hello Japan"に変更したときに、
変更していない"Hello"も確認する必要が出てきます。

//blankline

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

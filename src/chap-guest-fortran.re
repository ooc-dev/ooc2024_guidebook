= Fortranによるオブジェクト指向プログラミング

//flushright{
暗黙の型宣言@implict_none
//}

== はじめに
Fortranは，世界最古の高級言語であるFORTRANを祖先に持つプログラミング言語であり，スーパーコンピュータ等の大型計算機で高性能な計算を達成するために用いられています．Fortranは学習の容易さと高速な動作を両立しており，情報工学を専門としない研究者や技術者に愛用されています．

Fortranでオブジェクトを取り扱えるようになってから約30年が経過しています．しかし，Fortranが対象とする問題において，オブジェクト指向プログラミングが積極的に適用される事例は多くありません．その理由には色々あるのでしょうが，要因の一つに情報不足があることは確実です．

Fortranユーザの興味は数値シミュレーションにあるわけですから，オブジェクト指向プログラミングを始めようとしたときに，「イヌもネコも哺乳類で･･･」と生物学的分類の話をされても，「オブジェクト同士がメッセージを交換しながら相互作用を･･･」という謎めいた記述@<fn>{note1}を見ても，全く理解できないというのが正直な所です．

本稿では，Fortranユーザがより親しみを持っている問題の実装を例として，Fortranによるオブジェクト指向プログラミングを簡単に紹介していきます．

//footnote[note1][数値シミュレーションにおいて，“オブジェクト”はその意味のとおり物体を表します．また，“メッセージ”という用語を用いた場合，異なるプロセス間でデータをやりとりするMPI（Message Passing Interface，分散メモリ計算機環境で並列化を行う標準的なフレームワーク）が連想され，それが理解を妨げます．]

== ベクトル空間

私たちが日常的に使っている加算
//texequation{
1 + 2 = 3
//}
やスカラ乗算
//texequation{
2\cdot 3 = 6
//}

//noindent
を一般化した概念としてベクトル空間があります．@<m>{n}次元ベクトル全体の集合を@<m>{\mathbb{R\}^n}と書く．@<m>{\boldsymbol{a\}\in \mathbb{R\}^n}, @<m>{\boldsymbol{b\}\in \mathbb{R\}^n}がそれぞれ縦ベクトル

//texequation{
\boldsymbol{a} =
\begin{pmatrix}
a_1\\
\vdots\\
a_n
\end{pmatrix},
\boldsymbol{b} =
\begin{pmatrix}
b_1\\
\vdots\\
b_n
\end{pmatrix}
//}

//noindent
を表し，@<m>{k\in \mathbb{R\}}とするとき，加法およびスカラ乗法は

//texequation{
\boldsymbol{a} + \boldsymbol{b} =
\begin{pmatrix}
a_1 + b_1\\
\vdots\\
a_n + b_n
\end{pmatrix}
//}

//texequation{
k\cdot\boldsymbol{a} =
\begin{pmatrix}
k\cdot a_1\\
\vdots\\
k\cdot a_n
\end{pmatrix}
//}

//noindent
によって定義されます．集合@<m>{\mathbb{R\}^n}に加法とスカラ乗法が定義され，ある性質を満たすものをベクトル空間といいます．

== Fortranによるn次元ベクトルの実装
さて，上記の@<m>{\boldsymbol{a\} + \boldsymbol{b\}}に用いられる加算演算子@<m>{+}と，要素同士の和@<m>{a_1 + b_1}に用いられる加算演算子@<m>{+}は同じものでしょうか？同様に，@<m>{k\cdot \boldsymbol{a\}}と@<m>{k\cdot a_1}に用いられる乗算演算子@<m>{\cdot}は同じものでしょうか？

私たちは，記号@<m>{+, \cdot}が同じであっても，使われる箇所によってその振る舞いが異なることを理解しています．この振る舞いを，Fortranで実装してみましょう．

//list[list 1][Fortranによるベクトル型の定義]{
module type_vector
    use, intrinsic :: iso_fortran_env
    implicit none
    private

    type, public :: vector
        real(real32), public, allocatable :: val(:)
    contains
        procedure, public, pass(l) :: add_vec_vec
        procedure, public, pass(r) :: mul_r32_vec

        generic :: operator(+) => add_vec_vec
        generic :: operator(*) => mul_r32_vec
    end type vector

contains
    function add_vec_vec(l, r) result(add)
        class(vector), intent(in) :: l ! +演算子左のベクトル
        class(vector), intent(in) :: r ! +演算子右のベクトル
        type(vector) :: add

        add%val = l%val + r%val
    end function add_vec_vec

    function mul_r32_vec(factor, r) result(mul)
        real(real32), intent(in) :: factor ! *演算子左のスカラ
        class(vector), intent(in) :: r ! *演算子右のベクトル
        type(vector) :: mul

        mul%val = factor*r%val
    end function mul_r32_vec
end module type_vector
//}

@<code>{module}（モジュール）は，プログラム単位（宣言や定義を含み，個別にコンパイルされるスコープ）の一つです．@<code>{contains}文の上部で定数や変数の宣言を行い，@<code>{contains}文の下部で関数やサブルーチンの定義を行います．関数とサブルーチンを総称して，手続とよびます．

ベクトル型は，クラスに相当する利用者定義派生型として定義します．利用者定義派生型の定義は@<code>{type}文を用いて行われます．派生型の中では成分（メンバ変数，フィールド）を宣言します．成分を宣言するだけであれば構造体と同様ですが，メンバ関数やメソッドに相当する手続は，派生型の中にある@<code>{contains}文の下部で定義します．ただし，Fortranの場合は，派生型の定義内で手続も定義するのではなく，同一モジュール内で定義済みの手続と派生型を結びつけることでそれを実現します．このことから，Fortranではメンバ関数やメソッドに相当するものを，type-bound procedure（型に束縛された手続，型束縛手続）とよびます．

一定の条件を満たした型束縛手続は，演算子を介して呼び出せます．それを設定しているのが@<code>{generic :: operator() =>}の箇所で，@<code>{+}演算子と@<code>{*}演算子を，加算とスカラー乗算を行う型束縛手続と関連付けています．

このように定義したベクトル型を使ったプログラム例は，@<list>{list2}のようになります．

//list[list2][ベクトル型の使用例]{
program main
    use :: type_vector
    implicit none

    type(vector) :: a, b, c
    a = vector([1.0, 2.0])
    b = vector([2.0, 3.0])
    c = a + b
    print *, c%val !   3.00000000       5.00000000

    c = 4.0*a
    print *, c%val !   4.00000000       8.00000000
end program main
//}

@<code>{+}演算子を用いたベクトル@<code>{a, b}同士の加算を行った結果，その要素同士の加算が計算されていることがわかります．要素同士の加算は，型束縛手続の中で同じく@<code>{+}演算子を用いて行われているわけですが，同じ@<code>{+}でもその役割が異なることを再現できています．

== さらなる発展にむけて
Fortranユーザにとってはなじみ深い例を用いて，Fortranにおけるオブジェクト指向プログラミングの方法を簡単に説明しました．これだけでは，オブジェクト指向プログラミングの利点は理解しにくいと思いますが，手も足も出ない状態から一歩足を踏み出す助けになれば幸いです．ユーザが増えることで様々な試みがなされ，その中から有益な知見が生まれてくることを期待しています．

最後に，著者が行っている試みを一つ紹介しておきます．著者は，アルゴリズムの理解が容易で試行錯誤しやすい，非圧縮性流れの数値シミュレーション用フレームワークを作成しています．

非圧縮性流れの数値シミュレーションにおける最も基本的なアルゴリズムであるFractional Step法は，以下の3本の式を順次計算していくことで，流れの時間変化を計算します．

//texequation{
\begin{aligned}
\boldsymbol{u}^* &= \boldsymbol{u}^{n} + \varDelta t \left[ -\left(\boldsymbol{u}^{n}\cdot\nabla\right)\boldsymbol{u}^{n} +\nu \nabla^2 \boldsymbol{u}^{n}\right]\\
\nabla^2p^{n+1} &= \frac{\rho}{\varDelta t}\nabla\cdot\boldsymbol{u}^{*}\\
\boldsymbol{u}^{n+1}  &= \boldsymbol{u}^{*} - \frac{\varDelta t}{\rho}\nabla p^{n+1}
\end{aligned}
//}

//noindent
@<m>{\boldsymbol{u\}}は速度，@<m>{p}は圧力，@<m>{\rho}, @<m>{nu}は流体の物性値，@<m>{\varDelta t}は計算アルゴリズムの都合で現れるパラメータです．

著者が開発しているフレームワークを用いると，上記の計算アルゴリズムは@<list>{list3}のように実装されます．数式っぽく書けるところは数式っぽく，そうでないところは自然言語的に記述できるように設計しています．

//list[list3][非圧縮性流れの計算を行うコード例]{
type(staggered_uniform_grid_2d_type), target :: grid !! 空間格子
type(vector_2d_type) :: u !! 速度
type(scalar_2d_type) :: p !! 圧力
type(vector_2d_type) :: u_aux !! 中間速度

type(vector_boundary_condition_type) :: BC_u !! 速度境界条件
type(scalar_boundary_condition_type) :: BC_p !! 圧力境界条件

! initialize
u = .init.(u .on. grid)
p = .init.(p .on. grid)

! 中間速度の計算
u_aux = (u + dt*(-((u.dot.nabla)*u) + kvisc*.laplacian.u)) &
        .impose. BC_u !&

! 圧力Poisson方程式の求解
p = .inverse.(( &
              (laplacian(p).with.BC_p) == (dens/dt*.div.u_aux)) &
              .using. BiCGStab() &
              .until. below_criterion &
             ) !&

! 速度の計算
u = (u_aux - dt/dens*.grad.p) .impose. BC_u !&

! ファイル出力
call output(p .as. vtr .to."p")
call output(u .as. vtr .to."u")
//}

このように書けたから何なんだ？という意見もあるとは思います．一方で，オブジェクト指向プログラミングができると，実装できるコードの幅が広がります．また，数値計算以外のプログラミングに関する情報は，どうしてもオブジェクト指向プログラミングを前提にしているところもあるため，オブジェクト指向プログラミングに親しむと，それらの情報が理解しやすくなるという利点もあります．少しでも興味があれば取り組んでみてください．


　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/implicit_none.jpg}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        暗黙の型宣言　https://twitter.com/implict\_none\\
    \end{minipage}
    \hspace{1ex}
//}
Fortranや数値計算に関する技術書を作成しています．

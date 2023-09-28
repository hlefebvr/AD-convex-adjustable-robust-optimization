# Facility Location Problem (FLP)

## Deriving the separation problem

For a given $\mathbf x\in X$ and a given $\mathbf\xi\in\Xi$, the second-stage problem reads

$$\begin{align}
    \min_{y,v} \  & \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) \\
    \text{s.t.} \ & \sum_{j\in V_2} y_{ij} = v_i & \forall i\in V_1 \\
    & \sum_{i\in V_1} y_{ij} = d_j(\xi_j) & \forall j\in V_2 \\
    & v_i \le q_ix_i & \forall i\in V_1 \\
    & y_{ij} \ge 0 & \forall (i,j)\in V_1\times V_2 \\
    & v_i\in \mathbb R & \forall i\in V_1
\end{align}$$

Using Lemma 2, for a given $(x_0,\mathbf x)\in\mathbb R\times X$, we have that there exists $\mathbf y\in Y(\mathbf x, \mathbf\xi)$ such that $x_0 \ge \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right)$ if, and only if, the following holds: for all $\alpha\in\mathbb R^{|V_1|}$, for all $\beta\in\mathbb R^{|V_2|}$, for all $\gamma\in\mathbb R^{|V_1|}_+$ and for all $\lambda_0 \ge 0$,

$$
    \inf_{y,v} \left\lbrace
        \sum_{i\in V_1} \alpha_i \left( \sum_{j\in V_2} y_{ij} - v_i \right)
        + \sum_{j\in V_2} \beta_j \left( \sum_{j\in V_2} y_{ij} - d_j(\xi_j) \right)
        + \sum_{i\in V_1} \gamma_i \left( v_i - q_ix_i \right)
        + \lambda_0 \left( \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) - x_0 \right)
    \right\rbrace
    \le 0.
$$

Re-arranging the terms, this formula is recast as

$$
    \sum_{i\in V_1}
    \sum_{j\in V_2}
    \inf_{y_{ij}\ge 0} \left\lbrace
        y_{ij}( \alpha_i + \beta_j + t_{ij}\lambda_0 )
    \right\rbrace
    +
    \sum_{i\in V_1}
    \inf_{v_i\in\mathbb R} \left\lbrace
        \lambda_0 F_{a,b}(v_i) + v_i ( -\alpha_i + \gamma_i )
    \right\rbrace
    - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
    + \sum_{i\in V_1} (\lambda_0f_i - \gamma_i q_i ) x_i
    - \lambda_0x_0.
$$

The first terms (over $y_{ij}$) are bounded below if and only if it holds

$$
\alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2.
$$

The second terms (over $v_i$) can be solved analytically. We first assume that $\lambda_0 = 0$. Then, the infimum is bounded from below if and only if it holds 

$$
    \gamma_i - \alpha_i = 0 \quad \forall i\in V_1.
$$

If $\lambda_0 \neq 0$. To simplify our work, we first observe that 

$$
    \begin{align}
        \inf_{v_i\in \mathbb R} \left\lbrace
            \lambda_0 F_{a,b}(v_i) + v_i ( -\alpha_i + \gamma_i )
        \right\rbrace
        &= - \sup_{v_i} \left\lbrace v_i(\alpha_i - \gamma_i) - \lambda_0 F_{a,b}(v_i) \right\rbrace \\
        &= - (\lambda_0F_{a,b})^* \left( \alpha_i - \gamma_i \right) \\
        &= -\lambda_0 F_{a,b}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right).
    \end{align}
$$

Thus, we only have to compute the convex conjugate of $F_{a,b}$ which is given by $F^\star_{a,b}(\pi) = \sup_{v} \lbrace \pi v - av - b v^2 \rbrace$. The maximum is obtained for $v^*$ such that the derivative is zero, i.e.,

$$
    \frac{d}{dv} ( \pi v - a v - bv^2 ) = 0
    \Leftrightarrow 
    \pi - a - 2bv = 0 
    \Leftrightarrow 
    v^* = \frac{\pi - a}{2b}.
$$

Plugin it into the original function, we obtain 

$$
    F_{a,b}^*(\pi) = \frac{1}{4b} \left( \pi - a  \right)^2.
$$

In turn, we obtain

$$
    -\lambda_0 F_{a,b}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right)
    =
    - \frac{ (\alpha_i - \gamma_i )^2 }{ 4b\lambda_0 } + \frac { a ( \alpha_i - \gamma_i ) }{ 2b } - \frac{\lambda_0 a^2}{ 4b }
$$

All in all, the separation problem if $\lambda_0 \neq 0$ is then given by

$$
    \begin{align}
        \max \  & \sum_{i\in V_1} \left( - \frac{ (\alpha_i - \gamma_i )^2 }{ 4b\lambda_0 } + \frac { a }{2b} ( \alpha_i - \gamma_i ) - \frac{\lambda_0 a^2}{ 4b } \right) 
        - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
        + \sum_{i\in V_1} (\lambda_0f_i - \gamma_i q_i ) x_i
        - \lambda_0x_0.
        \\
        \text{s.t.} \  & \alpha_i + \beta_j + t_{ij}\lambda_0 = 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & (\lambda_0,\mathbf\alpha,\mathbf\beta,\mathbf\gamma) \in \Lambda \\
        & \mathbf\xi\in\Xi
    \end{align}
$$

Re-arranging the terms, one obtains the following model in which we introduced $z_i$ representing the ratio in the objective function:

$$
    \begin{align}
        \max \  & \sum_{i\in V_1} \left( - z_i + \frac { a }{2b} ( \alpha_i - \gamma_i ) - \frac{\lambda_0 a^2}{ 4b } \right) 
        - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
        + \sum_{i\in V_1} (\lambda_0f_i - \gamma_i q_i ) x_i
        - \lambda_0x_0 \\
        \text{s.t.} \  & \alpha_i + \beta_j + t_{ij}\lambda_0 = 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & (\alpha_i - \gamma_i)^2 \le 4bz_i\lambda_0 \quad \forall i\in V_1 \\
        & (\lambda_0,\mathbf\alpha,\mathbf\beta,\mathbf\gamma) \in \Lambda \\
        & \mathbf\xi\in\Xi, \\
        & z \ge 0,
    \end{align}
$$

with $\Lambda = \lbrace (\lambda_0, \alpha, \beta, \gamma) \in \mathbb R_+ \times \mathbb R^{|V_1|}\times \mathbb R^{|V_2|}\times \mathbb R^{|V_1|}_+ : \lVert (\lambda_0, \alpha, \beta, \gamma) \rVert \le 1 \rbrace$.
Observe how this model, derived for $\lambda_0 \neq 0$, implies $\alpha_i - \gamma_i = 0$ if $\lambda_0 = 0$.

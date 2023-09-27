# Facility Location Problem

## Deriving the separation problem

For a given $\bm x\in X$ and a given $\bm\xi\in\Xi$, the second-stage problem reads
$$\begin{align}
    \min_{y,v} \  & \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) \\
    \text{s.t.} \ & \sum_{j\in V_2} y_{ij} = v_i & \forall i\in V_1 \\
    & \sum_{i\in V_1} y_{ij} = d_j(\xi_j) & \forall j\in V_2 \\
    & v_i \le q_ix_i & \forall i\in V_1 \\
    & y_{ij} \ge 0 & \forall (i,j)\in V_1\times V_2 \\
    & v_i\ge 0 & \forall i\in V_1
\end{align}$$

Using Lemma 2, for a given $(x_0,\bm x)\in\mathbb R\times X$, we have that there exists $\bm y\in Y(\bm x, \bm\xi)$ such that $x_0 \ge \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right)$ if and only if the following holds: for all $\alpha\in\mathbb R^{|V_1|}$, for all $\beta\in\mathbb R^{|V_2|}$ and for all $\gamma\in\mathbb R^{|V_1|}_+$,
$$
    \inf_{y,v\ge 0} \left\{
        \sum_{i\in V_1} \alpha_i \left( \sum_{j\in V_2} y_{ij} - v_i \right)
        + \sum_{j\in V_2} \beta_j \left( \sum_{j\in V_2} y_{ij} - d_j(\xi_j) \right)
        + \sum_{i\in V_1} \gamma_i \left( v_i - q_ix_i \right)
        + \lambda_0 \left( \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) - x_0 \right).
    \right\}
$$

Re-arranging the terms, this formula is recast as
$$
    \sum_{i\in V_1}
    \sum_{j\in V_2}
    \inf_{y_{ij}\ge 0} \left\{
        y_{ij}( \alpha_i + \beta_j + t_{ij}\lambda_0 )
    \right\}
    +
    \sum_{i\in V_1}
    \inf_{v_i\ge 0} \left\{
        \lambda_0 F_{a,b}(v_i) + v_i ( -\alpha_i + \gamma_i )
    \right\}
    - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
    - \sum_{i\in V_1} (\gamma_i q_i + \lambda_0f_i ) x_i
    - \lambda_0x_0.
$$

The first terms (over $y_{ij}$) are bounded below if and only if it holds
$$
\alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2.
$$

The second terms (over $v_i$) can be solved analytically. We first assume that $\lambda_0 \neq 0$. To simplify our work, we first observe that 
$$
    \begin{align}
        \inf_{v_i\ge 0} \left\{
            \lambda_0 F_{a,b}(v_i) + v_i ( -\alpha_i + \gamma_i )
        \right\}
        &= - \sup_{v_i} \left\{ v_i(\alpha_i - \gamma_i) - \lambda_0 F_{a,b}(v_i) \right\} \\
        &= - (\lambda_0F_{a,b})^* \left( \alpha_i - \gamma_i \right) \\
        &= -\lambda_0 F_{a,b}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right).
    \end{align}
$$

Thus, we only have to compute the convex conjugate of $F_{a,b}$ which is given by 
$$
    F^*_{a,b}(\pi) = \sup_{v} \{ \pi v - av - bv^2 \}.
$$

The maximum is obtained for $v^*$ such that the derivative is zero, i.e.,
$$
    \frac{d}{dv} ( \pi v - a v - bv^2 ) = 0
    \Leftrightarrow 
    \pi - a - 2bv = 0 
    \Leftrightarrow 
    x = \frac{\pi - a}{2b}.
$$
Plugin it into the original function, we obtain 
$$
    F_{a,b}^*(\pi) = \frac{1}{4b} \left( \pi - a  \right)^2.
$$

In turn, we obtain
$$
    -\lambda_0 F_{a,b}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right)
    =
    - \frac{ (\alpha_i - \gamma_i )^2 }{ \lambda_0 } + 2a ( \alpha_i - \gamma_i ) - \lambda_0 a^2
$$

All in all, the separation problem is then 
$$
    \begin{align}
        \max \; & \sum_{i\in V_1} \left( - \frac{ (\alpha_i - \gamma_i )^2 }{ \lambda_0 } + 2a ( \alpha_i - \gamma_i ) - \lambda_0 a^2 \right) 
        - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
        - \sum_{i\in V_1} (\gamma_i q_i + \lambda_0f_i ) x_i
        - \lambda_0x_0.
        \\
        \text{s.t.} \; & \alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & (\lambda_0,\bm\alpha,\bm\beta,\bm\gamma) \in \Lambda \\
        & \bm\xi\in\Xi
    \end{align}
$$

Re-arranging the terms, one obtains the following model in which we introduced $z_i$ representing the ratio in the objective function:
$$
    \begin{align}
        \max \; & \sum_{i\in V_1} \left( -z_i + 2a(\alpha - \gamma_i) - \gamma_iq_i \right)
        - \sum_{j\in V_2} \beta_j d_j(\xi_j)
        - \lambda_0 \left( x_0 + |V_1|a^2 + \sum_{i\in V_1} f_ix_i \right) \\
        \text{s.t.} \; & \alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & (\alpha_i - \gamma_i)^2 \le z_i\lambda_0 \quad \forall i\in V_1 \\
        & (\lambda_0,\bm\alpha,\bm\beta,\bm\gamma) \in \Lambda \\
        & \bm\xi\in\Xi.
    \end{align}
$$

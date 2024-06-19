# Facility Location Problem (FLP)

## Deriving the separation problem

For a given $\mathbf x\in X$ and a given $\mathbf\xi\in\Xi$, the second-stage problem reads

$$\begin{align}
    \min_{y,v} \  & \sum_{i\in V_1} \left( f_ix_i + F_{i}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) \\
    \text{s.t.} \ & \sum_{j\in V_2} y_{ij} = v_i & \forall i\in V_1 \\
    & \sum_{i\in V_1} y_{ij} = d_j(\xi_j) & \forall j\in V_2 \\
    & v_i \le q_ix_i & \forall i\in V_1 \\
    & y_{ij} \ge 0 & \forall (i,j)\in V_1\times V_2 \\
    & v_i\in \mathbb R & \forall i\in V_1
\end{align}$$

Using Lemma 2, for a given $(x_0,\mathbf x)\in\mathbb R\times X$, we have that there exists $(\mathbf y, \mathbf v)\in Y(\mathbf x, \mathbf\xi)$ such that $x_0 \ge \sum_{i\in V_1} \left( f_ix_i + F_{a,b}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right)$ if, and only if, the following holds: for all $\alpha\in\mathbb R^{|V_1|}$, for all $\beta\in\mathbb R^{|V_2|}$, for all $\gamma\in\mathbb R^{|V_1|}_+$ and for all $\lambda_0 \ge 0$,

$$
    \inf_{y,v} \left\lbrace
        \sum_{i\in V_1} \alpha_i \left( \sum_{j\in V_2} y_{ij} - v_i \right)
        + \sum_{j\in V_2} \beta_j \left( \sum_{j\in V_2} y_{ij} - d_j(\xi_j) \right)
        + \sum_{i\in V_1} \gamma_i \left( v_i - q_ix_i \right)
        + \lambda_0 \left( \sum_{i\in V_1} \left( f_ix_i + F_{i}(v_i) + \sum_{j\in V_2} t_{ij}y_{ij}  \right) - x_0 \right)
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
            \lambda_0 F_{i}(v_i) + v_i ( -\alpha_i + \gamma_i )
        \right\rbrace
        &= - \sup_{v_i} \left\lbrace v_i(\alpha_i - \gamma_i) - \lambda_0 F_{i}(v_i) \right\rbrace \\
        &= - (\lambda_0F_{i})^* \left( \alpha_i - \gamma_i \right) \\
        &= -\lambda_0 F_{i}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right).
    \end{align}
$$

Thus, we only have to compute the convex conjugate of $F_{i}$. We note that 

$$ -F^\star_{i}(\pi) = \min_{v} \left\lbrace -\pi v - a_i + \frac{a_i(q_i + \varepsilon)}{ q_i - v + \varepsilon } \right\rbrace .$$

The minimum is obtained for $v^*$ such that the derivative is zero, i.e.,

$$
    \begin{align*}
        \frac{d}{dv} \left( -\pi v - a_i + \frac{a_i(q_i + \varepsilon)}{ q_i - v + \varepsilon } \right) = 0
        & \iff
        \frac{a_i(q_i + \varepsilon)}{ (q_i - v + \varepsilon)^2 } - \pi = 0 \\
        & \iff a_i(q_i+\varepsilon) = \pi(q_i - v + \varepsilon)^2 \\
        & \iff \sqrt{a_i(q_i+\varepsilon)} = \sqrt{\pi} (q_i - v + \varepsilon) \\
        & \iff v = -\frac{\sqrt{a_i(q_i+\varepsilon)}}{\sqrt{\pi}} + q_i + \varepsilon
    \end{align*}
$$

Plugin it into the original function, we obtain 

$$
    \begin{align}
        -F_{i}^*(\pi) 
        & = -\pi\left( -\frac{\sqrt{a_i(q_i+\varepsilon)}}{\sqrt{\pi}} + q_i + \varepsilon \right) -a_i + \frac{a_i(q_i + \varepsilon)}{ q_i - \left( -\frac{\sqrt{a_i(q_i+\varepsilon)}}{\sqrt{\pi}} + q_i + \varepsilon \right) + \varepsilon } \\
        & = \sqrt{\pi} \underbrace{\sqrt{ a_i(q_i + \varepsilon) }}_{:= c_i} -\pi(q_i + \varepsilon) - a_i + \frac{ \sqrt{\pi} a_i(q_i+\varepsilon) }{ \sqrt{a_i(q_i+\varepsilon)} } \\
        & = \sqrt{\pi} c_i -\pi(q_i + \varepsilon)  - a_i + \sqrt{\pi} \sqrt{ a_i(q_i+\varepsilon) } \\
        & = 2c_i\sqrt{\pi} -\pi(q_i + \varepsilon)  - a_i
    \end{align}
$$

In turn, we obtain

$$
    \begin{align}
    -\lambda_0 F_{i}^*\left( \frac{ \alpha_i - \gamma_i }{ \lambda_0 } \right)
    & = 2c_i\sqrt{\lambda_0(\alpha_i - \gamma_i) } + (\gamma_i - \alpha_i)(q_i + \varepsilon) -a\lambda_0
    \end{align}
$$

All in all, the separation problem if $\lambda_0 \neq 0$ is then given by

$$
    \begin{align}
        \max \  & \sum_{i\in V_1} \left( 2c_i\sqrt{\lambda_0(\alpha_i - \gamma_i) } + (\gamma_i - \alpha_i)(q_i + \varepsilon) -a\lambda_0 \right) 
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
        \max \  & \sum_{i\in V_1} \left( 2c_iz_i + (\gamma_i - \alpha_i)(q_i + \varepsilon) -a\lambda_0 \right) 
        - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
        + \sum_{i\in V_1} (\lambda_0f_i - \gamma_i q_i ) x_i
        - \lambda_0x_0 \\
        \text{s.t.} \  & \alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & z_i \le \sqrt{\lambda_0(\alpha_i - \gamma_i)} \quad \forall i\in V_1 \\
        & (\lambda_0,\mathbf\alpha,\mathbf\beta,\mathbf\gamma) \in \Lambda \\
        & \mathbf\xi\in\Xi, \\
        & z \ge 0,
    \end{align}
$$

with $\Lambda = \lbrace (\lambda_0,\alpha,\beta,\gamma) \in \mathbb R_+ \times \mathbb R^{|V_1|}\times \mathbb R^{|V_2|}\times \mathbb R^{|V_1|}_+ : \lVert (\lambda_0,\alpha,\beta,\gamma) \rVert \le 1 \rbrace$.

To account for the case $\lambda_0 = 0$, we need to ensure that $\lambda_0$ implies $\alpha_i - \gamma_i = 0$. To this end, we introduce new variables $s_i$ and add the following constraint:

$$
    (\alpha_i - \gamma_i)^2 \le \lambda_0 s_i \qquad \forall i\in V_1.
$$

All in all, the separation problem reads 

$$
    \begin{align}
        \max \  & \sum_{i\in V_1} \left( 2c_iz_i + (\gamma_i - \alpha_i)(q_i + \varepsilon) -a\lambda_0 \right) 
        - \sum_{j\in V_2} \beta_j d_j(\xi_j) 
        + \sum_{i\in V_1} (\lambda_0f_i - \gamma_i q_i ) x_i
        - \lambda_0x_0 \\
        \text{s.t.} \  & \alpha_i + \beta_j + t_{ij}\lambda_0 \ge 0 \quad \forall (i,j) \in V_1\times V_2, \\
        & z_i \le \sqrt{\lambda_0(\alpha_i - \gamma_i)} \quad \forall i\in V_1 \\
        & (\alpha_i - \gamma_i)^2 \le \lambda_0 s_i \quad \forall i\in V_1 \\
        & (\lambda_0,\mathbf\alpha,\mathbf\beta,\mathbf\gamma) \in \Lambda \\
        & \mathbf\xi\in\Xi, \\
        & z \ge 0,
    \end{align}
$$

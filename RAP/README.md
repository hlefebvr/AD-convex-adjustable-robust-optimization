# Resource Allocation Problem (RAP)

## Deriving the separation problem

For a given $\mathbf x\in X$ and a given $\mathbf\xi\in\Xi$, the second-stage problem reads

$$
    \begin{align}
        \min \ & \sum_{i\in V_1} c_ix_i \\
        \text{s.t.} \ & \sum_{j\in V_2} y_{ij} = v_i \\
        & \sum_{i\in V_1} \mu_{ij}y_{ij} \ge d_j(\xi_j) & \forall j\in V_2 \\
        & F_{1,b_i}(v_i) \le x_i & \forall i\in V_1  \\
        & v_i \in \mathbb R & \forall i\in V_1 \\
        & y_{ij} \ge 0 & \forall (i,j)\in V_1\times V_2
    \end{align}
$$

Using Lemma 2, for a given $(x_0,\mathbf x)\in\mathbb R\times X$, we have that there exists $(\mathbf y, \mathbf v) \in Y(\mathbf x, \mathbf \xi)$ such that $x_0 \ge \sum_{i\in V_1} c_ix_i$ if, and only if, the following holds: for all $\alpha\in\mathbb R^{|V_1|}$, for all $\beta\in\mathbb R_+^{|V_2|}$, for all $\gamma\in\mathbb{R}^{|V_1|}_+$ and for all $x_0 \ge 0$,

$$
    \inf_{y,v} \left\lbrace
        \sum_{i\in V_1} \alpha_i \left( \sum_{j\in V_2} y_{ij} - v_i \right)
        + \sum_{j\in V_2} \beta_j \left( d_j(\xi_j) - \sum_{i\in V_1} \mu_{ij} y_{ij} \right)
        + \sum_{i\in V_1} \gamma_i \left( F_{1,b_i}(v_i) - x_i \right)
        + \lambda_0 \left( \sum_{i\in V_1} c_ix_i - x_0 \right)
    \right\rbrace
    \le 0.
$$

Re-arranging the terms, this formula is recast as 

$$
    \sum_{i\in V_1} \sum_{j\in V_2} \inf_{ y_{ij} \ge 0 } \left\lbrace y_{ij}( \alpha_i - \mu_{ij}\beta_j ) \right\rbrace
    + \sum_{i\in V_1} \inf_{ v_{i} \in \mathbb R } \left\lbrace \gamma_i F_{1,b_i}(v_i) -\alpha_i v_{i} \right\rbrace
    + \sum_{j\in V_2} \beta_j d_j(\xi_j)
    - \sum_{i\in V_1} \gamma_i x_i
    + \lambda_0 \left( \sum_{i\in V_1} c_ix_i - x_0 \right).
$$

The first terms (over $y_{ij}$) are bounded below if, and only if, it holds

$$
    \alpha_i - \mu_{ij}\beta_j \ge 0 \quad \forall (i,j) \in V_1\times V_2.
$$

The second terms (over $v_i$) can be solved analytically. First, we assume that $\gamma_i = 0$. Then, the infimum is bounded from below if, and only if, it holds 

$$
    \alpha_i = 0 \quad \forall i\in V_1.
$$

If $\gamma_i \neq 0$. To simplify our work, we first observe that 

 ```math
    \begin{align}
        \inf_{ v_i\in \mathbb{R} } \left\lbrace 
            \gamma_i F_{1,b_i}(v_i) -\alpha_i v_i
        \right\rbrace
        &= - \sup_{ v_i \in \mathbb{R} } \left\lbrace \alpha_i v_i - \gamma_i F_{1,b_i}(v_i) \right\rbrace \\
        &= -(\gamma_i F_{1,b_i}(v_i))^*(\alpha_i) \\
        &= -\gamma_i F^*_{1,b_i}\left( \frac{\alpha_i}{\gamma_i} \right).
    \end{align}
```

Thus, we only have to compute the convex conjugate of $F_{1,b_i}$.

Recall that $F^\star_{a,b}(\pi) = \sup_{v} \lbrace \pi v - av - b v^2 \rbrace$. The maximum is obtained for $v^*$ such that the derivative is zero, i.e.,

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

Thus,

$$
    F_{1,b_i}^*(\pi) = \frac{1}{4b_i}(\pi - 1)^2.
$$

In turn, we obtain

$$
    -\gamma_i F_{1,b_i}^*\left( \frac{\alpha_i}{\gamma_i} \right)
    = - \frac{ \alpha_i^2 }{ 4b_i\gamma_i } + \frac{ \alpha_i }{ 2b_i } - \frac{ \gamma_i }{ 4 b_i }
$$

All in all, the separation problem if $\gamma_i \neq 0$ is then given by

$$
    \begin{align}
        \max \ & \sum_{i\in V_1} \left( - \frac{ \alpha_i^2 }{ 4b_i\gamma_i } + \frac{ \alpha_i }{ 2b_i } - \frac{ \gamma_i }{ 4 b_i } \right) 
        + \sum_{j\in V_2} \beta_j d_j(\xi_j)
        - \sum_{i\in V_1} \gamma_i x_i
        + \lambda_0 \left( \sum_{i\in V_1} c_ix_i - x_0 \right) \\
        \text{s.t.} \ & \alpha_i - \mu_{ij}\beta_j \ge 0 \quad \forall (i,j) \in V_1\times V_2 \\
        & (\lambda_0,\mathbf \alpha,\mathbf \beta,\mathbf \gamma) \in \Lambda \\
        & \mathbf \xi \in \Xi.
    \end{align}
$$

Introducing variables $z_i$ representing the ratio in the objective function, we obtain:

$$
    \begin{align}
        \max \ & \sum_{i\in V_1} \left( - z_i + \frac{ \alpha_i }{ 2b_i } - \frac{ \gamma_i }{ 4 b_i } \right) 
        + \sum_{j\in V_2} \beta_j d_j(\xi_j)
        - \sum_{i\in V_1} \gamma_i x_i
        + \lambda_0 \left( \sum_{i\in V_1} c_ix_i - x_0 \right) \\
        \text{s.t.} \ & \alpha_i - \mu_{ij}\beta_j \ge 0 \quad \forall (i,j) \in V_1\times V_2 \\
        & (\lambda_0,\mathbf\alpha,\mathbf\beta,\mathbf\gamma) \in \Lambda \\
        & \alpha_i^2 \le 4b_iz_i\gamma_i \quad \forall i\in V_1 \\
        & \mathbf \xi \in \Xi \\
        & z \ge 0,
    \end{align}
$$

with $`\Lambda = \lbrace (\lambda_0,\alpha,\beta,\gamma) \in \mathbb{R}_+ \times \mathbb{R}^{|V_1|} \times \mathbb{R}_+^{|V_2|} \times \mathbb{R}_+^{|V_1|} : \lVert (\lambda_0,\alpha,\beta,\gamma) \rVert \le 1 \rbrace`$. Observe how this model, derived for $\lambda_0 \neq 0$, implies $\alpha_i = 0$ if $\gamma_i = 0$.

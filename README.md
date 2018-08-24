# proteoformR

**proteoformR** seeks to provide a simple interface to recognize changepoints in peptide level measurements accross a protein, and allow these analyses to scale to proteome level datasets. Theoretically, if a user can define a continuously valued property for a series of peptides in a protein, then this protein could be probed for breakpoints. Research currently occuring in the [Villén Lab](http://villenlab.gs.washington.edu/) has shown that many of these breakpoints correspond to known cleavage events and protein domains.

This project is currently being tested with experimental data within our lab and more features will be coming soon.

# Installation

#### Prerequisite

```r
install.packages('devtools')
```

#### Github installation


```r
require(devtools)
install_github("AnthonyOfSeattle/proteoformR")
```

#### Local installation

You can clone this project and install it locally in your computer.


```r
require(devtools)
install("./proteoformR")
```

# Theory

The assumed input to **proteoformR** is a tidy dataframe containing normally distributed expression values, $y_{p,r,k}$ of replicated, $k \in \{1 , \ldots , K\}$, measurements of peptides, $p \in \{1 , \ldots , P_r\}$, from a set of reference proteins, $r \in \{1 , \ldots , R\}$. The changes in $y_{p,r,k}$ are modeled with the piecewise linear equation,

\[
y_{p,r,k} =  \alpha_{r,k} + f(p,r) + \epsilon
\]

\[
f(p,r) = \begin{cases} 
      \mu_{0,r} & p \in [1,b_0]            \\
      ...                                  \\
      \mu_{n,r} & p \in (b_{n-1}, b_{n}] \\
      ...                                  \\
      \mu_{N,r} & p \in (b_{N-1}, b_{N} = P_{r}]
   \end{cases}
\]

where the $\alpha_{r,k}$ are the replicate means for a reference and each $\mu_{n,r}$ represents the mean of a block of peptides parametrized by the location of $N$ changepoints along a protein, $b \in \{1 , \ldots , N-1\}$.
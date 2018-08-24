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
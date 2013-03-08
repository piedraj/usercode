
 rm -rf png
 rm -rf tex

 root -l -b -q XS.C+\(MET,MCmode,1,1\)
 root -l -b -q XS.C+\(MET,PPFmode,1,1\)

 root -l -b -q XS.C+\(Exactly3Leptons,MCmode,0,1\)
 root -l -b -q XS.C+\(Exactly3Leptons,PPFmode,0,1\)

 root -l -b -q XS.C+\(HasZCandidate,MCmode,0,1\)
 root -l -b -q XS.C+\(HasZCandidate,PPFmode,0,1\)

 root -l -b -q XS.C+\(MET,MCmode,0,1\)
 root -l -b -q XS.C+\(MET,PPFmode,0,1\)

 root -l -b -q XS.C+\(SSLike,MCmode,0,1\)
 root -l -b -q XS.C+\(SSLike,PPFmode,0,1\)

 root -l -b -q XS.C+\(SSLikeAntiBtag,MCmode,0,1\)
 root -l -b -q XS.C+\(SSLikeAntiBtag,PPFmode,0,1\)

 root -l -b -q drawScaleFactors.C+

 pdflatex mc_samples_wz8tev.tex
 pdflatex mc_samples_wz8tev.tex

 pdflatex yields.tex
 pdflatex yields.tex

 rm -rf *aux
 rm -rf *d
 rm -rf *log
 rm -rf *so
 rm -rf *~


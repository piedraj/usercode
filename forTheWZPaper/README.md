# SMP-14-014 figures

The xml files of the Feynman diagrams in Figure 1 have been created with JaxoDraw-2.1-0. To create the pdf versions we have installed the jaxodraw-pdf-plugin-1.0-0.jar plugin.

    java -jar jaxodraw-2.1-0.jar

The remaining figures of the paper have been created with ROOT.

    root -l -b -q drawFigure2.C
    root -l -b -q drawFigure3.C

    root -l -b -q "drawFigures4and5.C(\"Zpt\")"
    root -l -b -q "drawFigures4and5.C(\"LeadingJetPt\")"
    root -l -b -q "drawFigures4and5.C(\"Njets\")"

    root -l -b -q "drawFigure6.C(0)"
    root -l -b -q "drawFigure6.C(1)"

    root -l -b -q "drawFigure7.C(\"dg0\")"
    root -l -b -q "drawFigure7.C(\"lam0\")"
    root -l -b -q "drawFigure7.C(\"dk0\")"


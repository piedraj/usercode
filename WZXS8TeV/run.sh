
 export RAW=0;
 export PPF=1;
 
 ./resetproof.sh -a
 endproof
 
 root -l -b -q RunPROOF_WZ.C\(\"DoubleElectron\",RAW\)
 root -l -b -q RunPROOF_WZ.C\(\"DoubleElectron\",PPF\)
 
 ./resetproof.sh -a
 endproof
 
 root -l -b -q RunPROOF_WZ.C\(\"DoubleMu\",RAW\)
 root -l -b -q RunPROOF_WZ.C\(\"DoubleMu\",PPF\)
 
 ./resetproof.sh -a
 endproof
 
 root -l -b -q RunPROOF_WZ.C\(\"DYJets_Madgraph\"\);   # line  3
 root -l -b -q RunPROOF_WZ.C\(\"ZJets_Madgraph\"\);    # line  4
 root -l -b -q RunPROOF_WZ.C\(\"ZbbToLL\"\);           # line  5
 
 ./resetproof.sh -a
 endproof
 
 root -l -b -q RunPROOF_WZ.C\(\"WbbToLNu\"\);          # line  7
 root -l -b -q RunPROOF_WZ.C\(\"WJets_Madgraph\"\);    # line  8
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToElNuMad\"\);   # line  9
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToMuNuMad\"\);   # line 10 
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToTauNuMad\"\);  # line 11
 root -l -b -q RunPROOF_WZ.C\(\"WgammaToLNuG\"\);      # line 12
 root -l -b -q RunPROOF_WZ.C\(\"ZgammaToLLG\"\);       # line 13

 ./resetproof.sh -a
 endproof

 root -l -b -q RunPROOF_WZ.C\(\"TTbar_Madgraph\"\);    # line 17
 root -l -b -q RunPROOF_WZ.C\(\"TW\"\);                # line 23
 root -l -b -q RunPROOF_WZ.C\(\"TbarW\"\);             # line 24
 root -l -b -q RunPROOF_WZ.C\(\"WW\"\);                # line 34
 root -l -b -q RunPROOF_WZ.C\(\"WZTo3LNu\"\);          # line 36
 root -l -b -q RunPROOF_WZ.C\(\"WZTo2L2QMad\"\);       # line 37

 ./resetproof.sh -a
 endproof 

 root -l -b -q RunPROOF_WZ.C\(\"ZZTo2L2QMad\"\);       # line 38
#root -l -b -q RunPROOF_WZ.C\(\"ZZ\"\);                # line 40 -- REPLACED
 root -l -b -q RunPROOF_WZ.C\(\"ggZZ2L2L\"\);          # line 43
 root -l -b -q RunPROOF_WZ.C\(\"ggZZ4L\"\);            # line 44
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2Mu2Tau\"\);         # line 45
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4E\"\);              # line 46
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2E2Tau\"\);          # line 47
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4Mu\"\);             # line 48
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2E2Mu\"\);           # line 49
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4Tau\"\);            # line 50
 root -l -b -q RunPROOF_WZ.C\(\"HZZ4L\"\);             # line 51

 ./resetproof.sh -a
 endproof

 root -l -b -q RunPROOF_WZ.C\(\"WWGJets\"\);           # line 52
 root -l -b -q RunPROOF_WZ.C\(\"WZZJets\"\);           # line 53
 root -l -b -q RunPROOF_WZ.C\(\"ZZZJets\"\);           # line 54
 root -l -b -q RunPROOF_WZ.C\(\"WWZJets\"\);           # line 55
 root -l -b -q RunPROOF_WZ.C\(\"WWWJets\"\);           # line 56
 root -l -b -q RunPROOF_WZ.C\(\"TTWJets\"\);           # line 57
 root -l -b -q RunPROOF_WZ.C\(\"TTZJets\"\);           # line 58
 root -l -b -q RunPROOF_WZ.C\(\"TTWWJets\"\);          # line 59
 root -l -b -q RunPROOF_WZ.C\(\"TTGJets\"\);           # line 60

 ./resetproof.sh -a
 endproof


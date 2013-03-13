
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"DoubleElectron\",0\);
 root -l -b -q RunPROOF_WZ.C\(\"DoubleElectron\",1\);
 
 ./resetproof.sh -a; qdel all; #endproof; 
 
 root -l -b -q RunPROOF_WZ.C\(\"DoubleMu\",0\);
 root -l -b -q RunPROOF_WZ.C\(\"DoubleMu\",1\);
 
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"DYJets_Madgraph\"\);   # line  3
 root -l -b -q RunPROOF_WZ.C\(\"ZJets_Madgraph\"\);    # line  4
 root -l -b -q RunPROOF_WZ.C\(\"ZbbToLL\"\);           # line  5
 
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"WbbToLNu\"\);          # line  7
 root -l -b -q RunPROOF_WZ.C\(\"WJets_Madgraph\"\);    # line  8
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToElNuMad\"\);   # line  9
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToMuNuMad\"\);   # line 10 
 root -l -b -q RunPROOF_WZ.C\(\"WGstarToTauNuMad\"\);  # line 11
 root -l -b -q RunPROOF_WZ.C\(\"WgammaToLNuG\"\);      # line 12
 root -l -b -q RunPROOF_WZ.C\(\"ZgammaToLLG\"\);       # line 13
 
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"TTbar_Madgraph\"\);    # line 17
 root -l -b -q RunPROOF_WZ.C\(\"TW\"\);                # line 23
 root -l -b -q RunPROOF_WZ.C\(\"TbarW\"\);             # line 24
 root -l -b -q RunPROOF_WZ.C\(\"WW\"\);                # line 34
 root -l -b -q RunPROOF_WZ.C\(\"WZTo3LNu\"\);          # line 36 ( W -> lnu, Z -> ll )
 root -l -b -q RunPROOF_WZ.C\(\"WZTo2L2QMad\"\);       # line 37 ( W -> qq,  Z -> ll )
 root -l -b -q RunPROOF_WZ.C\(\"WZTo2QLNuMad\"\);      # line 38 ( W -> lnu, Z -> bb )
 
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"ZZTo2L2QMad\"\);       # line 39
 root -l -b -q RunPROOF_WZ.C\(\"ZZ\"\);                # line 41
 root -l -b -q RunPROOF_WZ.C\(\"ggZZ2L2L\"\);          # line 44
 root -l -b -q RunPROOF_WZ.C\(\"ggZZ4L\"\);            # line 45
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2Mu2Tau\"\);         # line 46
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4E\"\);              # line 47
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2E2Tau\"\);          # line 48
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4Mu\"\);             # line 49
 root -l -b -q RunPROOF_WZ.C\(\"ZZ2E2Mu\"\);           # line 50
 root -l -b -q RunPROOF_WZ.C\(\"ZZ4Tau\"\);            # line 51
 root -l -b -q RunPROOF_WZ.C\(\"HZZ4L\"\);             # line 52
 
 ./resetproof.sh -a; qdel all; #endproof;
 
 root -l -b -q RunPROOF_WZ.C\(\"WWGJets\"\);           # line 53
 root -l -b -q RunPROOF_WZ.C\(\"WZZJets\"\);           # line 54
 root -l -b -q RunPROOF_WZ.C\(\"ZZZJets\"\);           # line 55
 root -l -b -q RunPROOF_WZ.C\(\"WWZJets\"\);           # line 56
 root -l -b -q RunPROOF_WZ.C\(\"WWWJets\"\);           # line 57
 root -l -b -q RunPROOF_WZ.C\(\"TTWJets\"\);           # line 58
 root -l -b -q RunPROOF_WZ.C\(\"TTZJets\"\);           # line 59
 root -l -b -q RunPROOF_WZ.C\(\"TTWWJets\"\);          # line 60
 root -l -b -q RunPROOF_WZ.C\(\"TTGJets\"\);           # line 61

 ./resetproof.sh -a; qdel all; #endproof;


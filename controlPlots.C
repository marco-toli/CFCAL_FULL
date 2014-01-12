{
  TFile *_file0 = TFile::Open("out.root");
  TTree* t = (TTree*)_file0->Get("tree");
  
  TCanvas* c;
  
  
  
  int nBinsX = 256;
  double xMin = -32.;
  double xMax = +32.;
  
  int nBinsY = 256;
  double yMin = -32.;
  double yMax = +32.;
  
  int nBinsZ = 280;
  double zMin = -35.;
  double zMax = +35.;
  
  
  
  std::vector<float>* depositionX = new std::vector<float>;
  std::vector<float>* depositionY = new std::vector<float>;
  std::vector<int>* depositionIX = new std::vector<int>;
  std::vector<int>* depositionIY = new std::vector<int>;
  std::vector<int>* depositionType = new std::vector<int>;
  std::vector<int>* depositionFineLayer = new std::vector<int>;
  t -> SetBranchAddress("depositionX",&depositionX);
  t -> SetBranchAddress("depositionY",&depositionY);
  t -> SetBranchAddress("depositionIX",&depositionIX);
  t -> SetBranchAddress("depositionIY",&depositionIY);
  t -> SetBranchAddress("depositionType",&depositionType);
  t -> SetBranchAddress("depositionFineLayer",&depositionFineLayer);
  
  
  
  //---------
  // xIndexing
  
  TH2F* h2_xIndexing = new TH2F("h2_xIndexing","",nBinsX,xMin,xMax,1,yMin,yMax);
  h2_xIndexing -> GetXaxis() -> SetTitle("x");
  h2_xIndexing -> GetYaxis() -> SetTitle("y");
  
  for(int entry = 0; entry < t->GetEntries(); ++entry)
  {
    t -> GetEntry(entry);
    for(unsigned int iVal = 0; iVal < (*depositionX).size(); ++iVal)
    {
      if( (*depositionType)[iVal] != 2 ) continue;
      if( (*depositionFineLayer)[iVal] != 0 ) continue;
      
      int bin = h2_xIndexing -> Fill((*depositionX)[iVal],(*depositionY)[iVal]);
      h2_xIndexing -> SetBinContent(bin,(*depositionIX)[iVal]+0.5);
    }
  }
  
  c = new TCanvas("c_xIndexing","fiber xIndexing");
  h2_xIndexing -> Draw("COLZ");
  h2_xIndexing -> Draw("text,same");
  
  
  
  //---------
  // yIndexing
  
  TH2F* h2_yIndexing = new TH2F("h2_yIndexing","",1,xMin,xMax,nBinsY,yMin,yMax);
  h2_yIndexing -> GetXaxis() -> SetTitle("x");
  h2_yIndexing -> GetYaxis() -> SetTitle("y");
  
  for(int entry = 0; entry < t->GetEntries(); ++entry)
  {
    t -> GetEntry(entry);
    for(unsigned int iVal = 0; iVal < (*depositionY).size(); ++iVal)
    {
      if( (*depositionType)[iVal] != 2 ) continue;
      if( (*depositionFineLayer)[iVal] != 1 ) continue;
      
      int bin = h2_yIndexing -> Fill((*depositionX)[iVal],(*depositionY)[iVal]);
      h2_yIndexing -> SetBinContent(bin,(*depositionIY)[iVal]+0.5);
    }
  }
  
  c = new TCanvas("c_yIndexing","fiber yIndexing");
  h2_yIndexing -> Draw("COLZ");
  h2_yIndexing -> Draw("text,same");
  
  
  
  //------------------
  // y vs x projection
  
  TH2F* h2_YvsX = new TH2F("h2_YvsX","",nBinsX,xMin,xMax,nBinsY,yMin,yMax);
  h2_YvsX -> GetXaxis() -> SetTitle("x");
  h2_YvsX -> GetYaxis() -> SetTitle("y");
  TH2F* h2_YvsX_localY_0 = new TH2F("h2_YvsX_localY_0","",nBinsX,xMin,xMax,nBinsY,yMin,yMax);
  h2_YvsX_localY_0 -> SetMarkerSize(0.1);
  h2_YvsX_localY_0 -> SetMarkerColor(kRed);
  TH2F* h2_YvsX_localY_1 = new TH2F("h2_YvsX_localY_1","",nBinsX,xMin,xMax,nBinsY,yMin,yMax);
  h2_YvsX_localY_1 -> SetMarkerSize(0.1);
  h2_YvsX_localY_1 -> SetMarkerColor(kGreen);
  
  t -> Draw("depositionY:depositionX >> h2_YvsX","depositionType == 2","goff");
  t -> Draw("depositionY:depositionX >> h2_YvsX_localY_0","depositionType == 2 && depositionFiberLocalY > 0.","goff");
  t -> Draw("depositionY:depositionX >> h2_YvsX_localY_1","depositionType == 2 && depositionFiberLocalY < 0.","goff");
  
  c = new TCanvas("c_YvsX","deposition y vs x");
  h2_YvsX -> Draw("COLZ");
  h2_YvsX_localY_0 -> Draw("same");
  h2_YvsX_localY_1 -> Draw("same");
  
  
  
  //------------------
  // z vs x projection
  
  TH2F* h2_ZvsX = new TH2F("h2_ZvsX","",nBinsX,xMin,xMax,nBinsZ,zMin,zMax);
  h2_ZvsX -> GetXaxis() -> SetTitle("x");
  h2_ZvsX -> GetYaxis() -> SetTitle("z");
  TH2F* h2_ZvsX_localXZ_0 = new TH2F("h2_ZvsX_localXZ_0","",nBinsX,xMin,xMax,nBinsZ,zMin,zMax);
  h2_ZvsX_localXZ_0 -> SetMarkerSize(0.1);
  h2_ZvsX_localXZ_0 -> SetMarkerColor(kRed);
  TH2F* h2_ZvsX_localXZ_1 = new TH2F("h2_ZvsX_localXZ_1","",nBinsX,xMin,xMax,nBinsZ,zMin,zMax);
  h2_ZvsX_localXZ_1 -> SetMarkerSize(0.1);
  h2_ZvsX_localXZ_1 -> SetMarkerColor(kGreen);
  
  t -> Draw("depositionZ:depositionX >> h2_ZvsX","depositionType == 2","goff");
  t -> Draw("depositionZ:depositionX >> h2_ZvsX_localXZ_0","depositionType == 2 && depositionFiberLocalX > 0. && depositionFiberLocalZ > 0.","goff");
  t -> Draw("depositionZ:depositionX >> h2_ZvsX_localXZ_1","depositionType == 2 && depositionFiberLocalX < 0. && depositionFiberLocalZ < 0.","goff");
  
  c = new TCanvas("c_ZvsX","deposition z vs x");
  h2_ZvsX -> Draw("COLZ");
  h2_ZvsX_localXZ_0 -> Draw("same");
  h2_ZvsX_localXZ_1 -> Draw("same");
  
  
  
  //------------------
  // y vs z projection
  
  TH2F* h2_YvsZ = new TH2F("h2_YvsZ","",nBinsZ,zMin,zMax,nBinsY,yMin,yMax);
  h2_YvsZ -> GetXaxis() -> SetTitle("z");
  h2_YvsZ -> GetYaxis() -> SetTitle("y");
  TH2F* h2_YvsZ_fineLayer0 = new TH2F("h2_YvsZ_fineLayer0","",nBinsZ,zMin,zMax,nBinsY,yMin,yMax);
  h2_YvsZ_fineLayer0 -> SetMarkerSize(0.1);
  h2_YvsZ_fineLayer0 -> SetMarkerColor(kRed);
  TH2F* h2_YvsZ_fineLayer1 = new TH2F("h2_YvsZ_fineLayer1","",nBinsZ,zMin,zMax,nBinsY,yMin,yMax);
  h2_YvsZ_fineLayer1 -> SetMarkerSize(0.1);
  h2_YvsZ_fineLayer1 -> SetMarkerColor(kGreen);
  
  t -> Draw("depositionY:depositionZ >> h2_YvsZ","depositionType == 2","goff");
  t -> Draw("depositionY:depositionZ >> h2_YvsZ_fineLayer0","depositionType == 2 && depositionFineLayer == 0","goff");
  t -> Draw("depositionY:depositionZ >> h2_YvsZ_fineLayer1","depositionType == 2 && depositionFineLayer == 1","goff");
  
  c = new TCanvas("c_YvsZ","deposition y vs z");
  h2_YvsZ -> Draw("COLZ");
  h2_YvsZ_fineLayer0 -> Draw("same");
  h2_YvsZ_fineLayer1 -> Draw("same");
  
  /*
  TH1F* h1   = new TH1F("h1",  "",250,0.,10000);
  TH1F* h2   = new TH1F("h2",  "",250,0.,10000);
  TH1F* h2_1 = new TH1F("h2_1","",250,0.,10000);
  TH1F* h2_2 = new TH1F("h2_2","",250,0.,10000);
  TH1F* h3   = new TH1F("h3",  "",250,0.,10000);
  TH1F* h3_0 = new TH1F("h3_0","",250,0.,10000);
  TH1F* h3_1 = new TH1F("h3_1","",250,0.,10000);
  TH1F* h3_2 = new TH1F("h3_2","",250,0.,10000);
  TH1F* h3_3 = new TH1F("h3_3","",250,0.,10000);
  TH1F* h3_4 = new TH1F("h3_4","",250,0.,10000);
  
  h1 -> SetLineWidth(2);
  
  h2 -> SetLineColor(kRed);
  h2_1 -> SetLineColor(kBlack);
  h2_2 -> SetLineColor(kBlack);
  
  h3   -> SetLineColor(kBlue);
  h3_0 -> SetLineColor(kBlue);
  h3_1 -> SetLineColor(kBlue);
  h3_2 -> SetLineColor(kBlue);
  h3_3 -> SetLineColor(kBlue);
  h3_4 -> SetLineColor(kBlue);
  h3_0 -> SetLineStyle(2);
  h3_1 -> SetLineStyle(2);
  h3_2 -> SetLineStyle(2);
  h3_3 -> SetLineStyle(2);
  h3_4 -> SetLineStyle(2);
  
  //t->Draw("opPhoton_time_gap     >>  h1",  "",                 "goff");
  t->Draw("opPhotonFast_time_gap >>  h2",  "",                 "goff");
  t->Draw("opPhotonFast_time_gap >>  h2_1",  "opPhotonFast_process_gap == 1","goff");
  t->Draw("opPhotonFast_time_gap >>  h2_2",  "opPhotonFast_process_gap == 2","goff");
  t->Draw("opPhotonFast_time_gap >>  h2",  "",                 "goff");
  //t->Draw("opPhoton_gapTime0     >>  h3",  "opPhoton_gapProb0","goff");
  //t->Draw("opPhoton_gapTime1     >>+ h3",  "opPhoton_gapProb1","goff");
  //t->Draw("opPhoton_gapTime2     >>+ h3",  "opPhoton_gapProb2","goff");
  //t->Draw("opPhoton_gapTime3     >>+ h3",  "opPhoton_gapProb3","goff");
  //t->Draw("opPhoton_gapTime4     >>+ h3",  "opPhoton_gapProb4","goff");
  //t->Draw("opPhoton_gapTime0     >>  h3_0","opPhoton_gapProb0","goff");
  //t->Draw("opPhoton_gapTime1     >>  h3_1","opPhoton_gapProb1","goff");
  //t->Draw("opPhoton_gapTime2     >>  h3_2","opPhoton_gapProb2","goff");
  //t->Draw("opPhoton_gapTime3     >>  h3_3","opPhoton_gapProb3","goff");
  //t->Draw("opPhoton_gapTime4     >>  h3_4","opPhoton_gapProb4","goff");
  
  TCanvas* c1 = new TCanvas("c1","c1");
  c1 -> SetLogy();
  
  //h1   -> Draw();
  h2   -> Draw("");
  h2_1 -> Draw("same");
  h2_2 -> Draw("same");
  //h3   -> Draw("same");
  //h3_0 -> Draw("same");
  //h3_1 -> Draw("same");
  //h3_2 -> Draw("same");
  //h3_3 -> Draw("same");
  //h1   -> Draw("same");
  //h2   -> Draw("same");
  */
}

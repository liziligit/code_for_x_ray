void four_xy()
// {
//     auto c1 = new TCanvas("c1", "c1", 600, 400);
//     auto hb1 = new TH2F("hb1", "Example of BOX plots with option SAME ", 40, -3, 3, 40, -3, 3);
//     auto hb2 = new TH2F("hb2", "hb2", 40, -3, 3, 40, -3, 3);
//     auto hb3 = new TH2F("hb3", "hb3", 40, -3, 3, 40, -3, 3);
//     auto hb4 = new TH2F("hb4", "hb4", 40, -3, 3, 40, -3, 3);
//     for (Int_t i = 0; i < 1000; i++)
//     {
//         double x, y;
//         gRandom->Rannor(x, y);
//         if (x > 0 && y > 0)
//             hb1->Fill(x, y, 4);
//         if (x < 0 && y < 0)
//             hb2->Fill(x, y, 4);
//         if (x > 0 && y < 0)
//             hb3->Fill(x, y, 1);
//         if (x < 0 && y > 0)
//             hb4->Fill(x, y, 1);
//         cout << x << ":" << y << endl;
//     }
//     hb1->SetFillColor(1);
//     hb2->SetFillColor(2);
//     hb3->SetFillColor(3);
//     hb4->SetFillColor(4);
//     hb1->Draw("box");
//     hb2->Draw("box same");
//     hb3->Draw("box same");
//     hb4->Draw("box same");
// }

{
   auto c03 = new TCanvas("c03","c03",700,400);
   gStyle->SetOptStat(0);
   auto htext3 = new TH2F("htext3","Several 2D histograms drawn with option TEXT",10,-4,4,10,-20,20);
   auto htext4 = new TH2F("htext4","htext4",10,-4,4,10,-20,20);
   auto htext5 = new TH2F("htext5","htext5",10,-4,4,10,-20,20);
   float px, py;
   for (Int_t i = 0; i < 25000; i++) {
      gRandom->Rannor(px,py);
      htext3->Fill(4*px,20*py,0.1);
      htext4->Fill(4*px,20*py,0.5);
      htext5->Fill(4*px,20*py,1.0);
   }
   htext4->SetMarkerSize(1.8);
   htext5->SetMarkerSize(1.8);
   htext5->SetMarkerColor(kRed);
   htext3->Draw("COL");
   htext4->SetBarOffset(0.2);
   htext4->Draw("TEXT SAME");
   htext5->SetBarOffset(-0.2);
   htext5->Draw("TEXT SAME");
}
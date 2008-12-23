/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2008 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#include <irtkImage.h>

#include <irtkEMClassification.h>

#include <irtkGaussian.h>

char *output_name;

void usage()
{
  cerr << "Usage: ems [image] [n] [atlas 1 ... atlas n] [output] <options>" << endl;
  exit(1);
}

int main(int argc, char **argv)
{
  int i, n, ok, padding, iterations;

  if (argc < 4) {
    usage();
    exit(1);
  }

  // Input image
  irtkRealImage image;
  image.Read(argv[1]);
  argc--;
  argv++;

  // Number of tissues
  n = atoi(argv[1]);
  argc--;
  argv++;

  // Probabilistic atlas
  irtkRealImage **atlas = new irtkRealImage*[n];
  irtkRealImage *background=NULL;

  // Read atlas for each tissue
  for (i = 0; i < n; i++) {
    atlas[i] = new irtkRealImage;
    atlas[i]->Read(argv[1]);
    cerr << "Image " << i <<" = " << argv[1] <<endl;
    argc--;
    argv++;
  }

  // File name for output
  output_name = argv[1];
  argc--;
  argv++;

  // Default parameters
  iterations = 15;
  padding    = -1;//MIN_GREY;

  // Parse remaining parameters
  while (argc > 1) {
    ok = False;
    if ((ok == False) && (strcmp(argv[1], "-iterations") == 0)) {
      argc--;
      argv++;
      iterations = atoi(argv[1]);
      argc--;
      argv++;
      ok = True;
    }
    if ((ok == False) && (strcmp(argv[1], "-padding") == 0)) {
      argc--;
      argv++;
      padding = atoi(argv[1]);
      argc--;
      argv++;
      ok = True;
    }
    if ((ok == False) && (strcmp(argv[1], "-background") == 0)) {
      argc--;
      argv++;
      background = new irtkRealImage;
      background->Read(argv[1]);
      cerr << "background  = " << argv[1] <<endl;
      argc--;
      argv++;
      ok = True;
    }
    if (ok == False) {
      cerr << "Can not parse argument " << argv[1] << endl;
      usage();
    }
  }

  //irtkEMClassification classification(n, atlas, background);
  irtkEMClassification classification(n, atlas);


  classification.SetInput(image);
  classification.SetPadding(padding);
  classification.InitialiseGMM();

  double rel_diff;
  //for (i = 0; rel_diff > 0.0001; i++){
  i=1;
  do {
    cout << "Iteration = " << i << " / " << iterations << endl;
    rel_diff = classification.IterateGMM(i);
    i++;
  } while ((rel_diff>0.001)&&(i<50));


  classification.WriteGaussianParameters("parameters.txt");

  irtkRealImage segmentation;
  classification.ConstructSegmentationNoBG(segmentation);
  segmentation.Write(output_name);
  if (n==11) {
    classification.WriteProbMap(0,"csf.hdr");
    classification.WriteProbMap(1,"gray.hdr");
    classification.WriteProbMap(2,"caudate.hdr");
    classification.WriteProbMap(3,"putamen.hdr");
    classification.WriteProbMap(4,"nigra.hdr");
    classification.WriteProbMap(5,"cerebellum.hdr");
    classification.WriteProbMap(6,"thalamus.hdr");
    classification.WriteProbMap(7,"pallidum.hdr");
    classification.WriteProbMap(8,"brainstem.hdr");
    classification.WriteProbMap(9,"white.hdr");
    classification.WriteProbMap(10,"cerebellum-white.hdr");
    classification.WriteProbMap(11,"other.hdr");
  }
  if (n==5) {
    classification.WriteProbMap(0,"caudate.hdr");
    classification.WriteProbMap(1,"putamen.hdr");
    classification.WriteProbMap(2,"thalamus.hdr");
    classification.WriteProbMap(3,"pallidum.hdr");
    classification.WriteProbMap(4,"white.hdr");
  }
  if (n==2) {
    classification.WriteProbMap(0,"gray.hdr");
    classification.WriteProbMap(1,"white.hdr");
  }

  if (n==3) {
    classification.WriteProbMap(0,"csf.hdr");
    classification.WriteProbMap(1,"gray.hdr");
    classification.WriteProbMap(2,"white.hdr");
  }

}


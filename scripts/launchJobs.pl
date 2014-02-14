use Env;



open(USERCONFIG,$ARGV[0]);
while(<USERCONFIG>)
{
  chomp;
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  my($var,$value) = split(/\s*=\s*/, $_, 2);
  $User_Preferences{$var} = $value;
}

$TBCfg     = $User_Preferences{"TBCfg"};
$GPSMac    = $User_Preferences{"GPSMac"};
$OUTPUTDir = $User_Preferences{"OUTPUTDir"};



open(LANCIA,">","lancia.sh") or die "Can't open file lancia.sh";

open(LIST,"./list_FNAL14.txt");
#open(LIST,"./list_Full_Lisbon.txt");
while(<LIST>)
{
  chomp;
  s/#.*//;                # no comments
  s/^\s+//;               # no leading white
  s/\s+$//;               # no trailing white
  
  ($label,$particle,$energy,$preL,$Nevts,$Njobs,$Nfirst) = split(" ");
  
  if( $label eq "" )
  {
    next;
  }
  
  if( $preL > 0. )
  {
    $label = $label."_preshower".$preL."mm";
  }
  print($label." ".$particle," ".$energy." ".$preL." ".$Nevts." ".$Njobs." ".$Nfirst."\n");
  
  $runDir = $OUTPUTDir."/run_".$label."/";
  if( ! -e $runDir )
  {
    #print("mkdir ".$runDir."\n");
    system("mkdir ".$runDir);
  }
  
  for($iJob = $Nfirst; $iJob < $Nfirst+$Njobs; ++$iJob)
  {
    $jobDir = $runDir."/job_".$iJob."/";
    if( ! -e $jobDir )
    {
      #print("mkdir ".$jobDir."\n");
      system("mkdir ".$jobDir);
    }
    
    else
    {
      if( $ARGV[1] == 1 )
      {
        #print("rm -rf ".$jobDir."\n");
        system("rm -rf ".$jobDir);
        #print("mkdir ".$jobDir."\n");
        system("mkdir ".$jobDir);
      }
      else
      {
        print(">>> output directory not empty. Remove it or run ./launchJobs.pl params.cfg 1 <<< \n");
        die;
      }
    }
    
    $TbCfg = $jobDir."/TB.cfg";
    system("cat ".$TBCfg."   | sed -e s%SEED%"."-1".
                         "%g | sed -e s%PRESHOWERL%".$preL.
                              "%g > ".$TbCfg);
    
    $jobGpsMac = $jobDir."/gps.mac";
    system("cat ".$GPSMac."   | sed -e s%PARTICLE%".$particle.
                          "%g | sed -e s%ENERGY%".$energy.
                          "%g | sed -e s%NEVTS%".$Nevts.
                          "%g > ".$jobGpsMac);
    $jobOut = $jobDir."/log_".$label."_".$iJob.".txt";
    
    $jobSh = $jobDir."/job_".$label."_".$iJob.".sh";
    open(JOBSH,">",$jobSh) or die "Can't open file ".$jobSh;
    
    print JOBSH "echo \$SHELL\n";
    print JOBSH "cd ".$jobDir."\n";
    print JOBSH "pwd \n";
    print JOBSH "unbuffer CFCAL_FULL ".$jobDir."TB.cfg out_".$iJob." >& ".$jobOut."\n";
    
    print LANCIA "qsub -d ".$jobDir." -q shortcms ".$jobSh."\n";
  }
}

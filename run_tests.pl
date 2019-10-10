#!/usr/bin/perl
#
# /home/ubuntu/datasets/results/PAWCS_result/baseline/highway/bin001612.png
#

use File::Basename;
my @exts = qw(.png .jpg);

$bin="./draw";
$dataset="/home/ubuntu/datasets/CD2014/dataset";

opendir(DIR, $dataset) || die "Error abriendo file $dataset\n";
@files = readdir(DIR);
closedir DIR;
foreach $key (@files) {
	next if($key eq "." || $key eq "..");
	next if(! -d "$dataset/$key");
	opendir(DIR2, "$dataset/$key");
	@files2 = readdir(DIR2);
	closedir DIR2;
	foreach $key2 (@files2) {
		next if($key2 eq "." || $key2 eq "..");
		print "$dataset/$key/$key2 2 $key2\n";
		next if($key2 ne "streetCornerAtNight");
		next if(! -d "$dataset/$key/$key2");
		opendir(DIR3, "$dataset/$key/$key2/groundtruth");
		@files3 = readdir(DIR3);
		closedir DIR3;
		foreach $key3 (@files3) {
			next if($key3 eq "." || $key3 eq "..");
			#print "$dataset/$key/$key2/$key3 ``\n";
			#print "$dataset/$key/$key2/groundtruth/$key3 -- \n";

			$filepath = "$dataset/$key/$key2/groundtruth/$key3";
			next if( -d $filepath );
			chomp $filepath;
			my ($name, $dir, $ext) = fileparse($filepath, @exts);
			#print "$name $dir $ext \n";
			next if($ext ne ".jpg" && $ext ne ".png");

			#print "$filepath\n";
			if( ! -d $dir."/annotations") {
				mkdir $dir."/annotations";
			}
			$cmd=$bin." ".$filepath." ".$dir."/annotations";
			#print $cmd."\n";
			system($cmd);
		}
	}
}

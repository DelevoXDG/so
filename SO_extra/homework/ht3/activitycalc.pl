#!/usr/bin/env perl

use utf8;
use Storable qw(dclone);

if (@ARGV == 0 && -t STDIN && -t STDERR) {
	print STDERR "$0: ERROR: No arguments given.\n";
	exit(-1);
}

open($FILE1, '<', @ARGV[0]) or die("Could not open file @ARGV[0]");
open($FILE2, '<', @ARGV[0]) or die("Could not open file @ARGV[0]");
(my $no_extension = @ARGV[0]) =~ s/\.[^.]+$//;
open($OUT, ">", "$no_extension.html");


binmode $FILE1, ':utf8';
binmode $FILE2, ':utf8';
binmode $OUT, ':utf8';
binmode STDOUT, ":utf8";
read $FILE2, $file_content, -s $FILE2;

@lol=('.','.','.','.');

print $OUT <<HEADER;
<html>
<head>
	<style>
		table,
		th,
		td {
			border: 1px solid black;
			border-collapse: collapse;
		}
	</style>
</head>
<table>
HEADER

@dates = ($file_content =~ /((?|0?[1-9]|[12][0-9]|3[01])[.](?|I(?|I{1,2}|[VX])?|VI{0,3}|X(?|I{1,3}|IV|V)?))/gi);
@names = ($file_content =~ /(\b[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\s[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\b)(?![\s\S]*\b\1\b)/gi);
@names = sort @names;

# @dates = ($file_content =~ /V/ig);

# print "@dates\n";
%students;
%activity;
for (@dates){
	$activity{$_} = 0;
}
$activity{'sum'}=0;
for(@names){

	# $small{$_} = $activity{$_} for keys %activity;
	my %hash_copy = %{$activity};
	for (@dates){
		$hash_copy{$_} = 0;
	}

	# %copy = dclone(\%activity);
	$students{$_} = \%hash_copy;
}

# for (@dates){
# 	print("$_","\n");
# }
# for (@names){
# 	print("$_","\n");
# }
$students{'Nowak Marcelina'}->{'8.IV'} += 1;
foreach my $student (@names) {

	# print "$student: $students{$student}\n";
	print "$student:\n";
	foreach my $date (@dates) {

		print "$date: $students{$student}->{$date}\n";
	}
}


print $OUT <<FOOTER;
</table>
</html>
FOOTER
close $FILE1;
close $OUT;

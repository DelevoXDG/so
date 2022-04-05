#!/usr/bin/env perl

use utf8;

# use Storable qw(dclone);

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
		th, td {
 		padding: 4px;
		padding-left: 8px;
		padding-right: 8px;
		}
		table.center {
 	 	margin-left: auto;
  		margin-right: auto;
		}
	</style>
</head>
<table style="background-color:#FFF8DC">
HEADER
$dateregex='((?|0?[1-9]|[12][0-9]|3[01])[.](?|I(?|I{1,2}|[VX])?|VI{0,3}|X(?|I{1,3}|IV|V)?))';
$nameregex='(\b[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\s[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\b)';
@dates = ($file_content =~ /$dateregex/gi);
@names = ($file_content =~ /$nameregex(?![\s\S]*\b\1\b)/gi);
@names = sort @names;

$cur_date;
$cur_name;

%students;
%activity;
for (@dates){
	$activity{$_} = 0;
}
$activity{sum}=0;
for(@names){

	# $small{$_} = $activity{$_} for keys %activity;
	my %hash_copy = %{$activity};
	for (@dates){
		$hash_copy{$_} = 0;
	}

	# %copy = dclone(\%activity);
	$students{$_} = \%hash_copy;
}
while (my $line = <$FILE1>) {
	if ($line =~ /$dateregex/g){
		($cur_date) = $1;
	}
	if ($line =~ /$nameregex/g){
		($cur_name) = $1;

		# print "$cur_name\n";
		$count = () = $line =~ /[+]/g;
		$students{$cur_name}->{$cur_date} += $count;
		$students{$cur_name}->{sum}+=$count;
	}
}


foreach my $student (@names) {

	# print "$student: $students{$student}\n";
	print "$student:\n";
	foreach my $date (@dates) {
		print "$date: $students{$student}->{$date}\n";
	}
	print "sum: $students{$student}->{sum}\n";
}

print $OUT "\t<tr>\n";
print $OUT "\t\t<th style=\"text-align:left\">Imie</th>\n";
print $OUT "\t\t<th style=\"text-align:left\">Nazwisko</th>\n";
foreach my $date (@dates) {
	print $OUT "\t\t<th style=\"text-align:left\">$date</th>\n";
}
print $OUT "\t\t<th style=\"text-align:left\">Suma</td>\n";
print $OUT "\t</tr>\n";

foreach my $student (@names) {
	print $OUT "\t<tr>\n";
	my @bits = split / /, $student;
	my $name = $bits[1];
	my $surname = $bits[0];
	print $OUT "\t\t<td>$name</td>\n";
	print $OUT "\t\t<td>$surname</td>\n";
	foreach my $date (@dates) {
		print $OUT "\t\t<td>$students{$student}->{$date}</td>\n";
	}
	print $OUT "\t\t<td>$students{$student}->{sum}</td>\n";
	print $OUT "\t</tr>\n";
}


print $OUT <<FOOTER;
</table>
</html>
FOOTER
close $FILE1;
close $OUT;

# $students{'Nowak Marcelina'}->{'8.IV'} += 1;
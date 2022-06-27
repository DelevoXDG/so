#!/usr/bin/env perl

use utf8;

# use Storable qw(dclone);

if (@ARGV == 0 && -t STDIN && -t STDERR) {
	print STDERR "$0: ERROR: No arguments given.\n";
	exit(-1);
}

open($file1, '<', @ARGV[0]) or die("Could not open file @ARGV[0]"); # To read line by line
open($file2, '<', @ARGV[0]) or die("Could not open file @ARGV[0]"); # To slurp
($basename = @ARGV[0]) =~ s{^.*/|\.[^.]+$}{}g; #get basename
open($out, ">", "$basename.html");

binmode $file1, ':utf8';
binmode $file2, ':utf8';
binmode $out, ':utf8';
read $file2, $file_content, -s $file2;	# Slurping file in string

# HTML boilerplate
print $out <<HEADER;
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

# Getting all the data with regex
my $dateregex='((?|0?[1-9]|[12][0-9]|3[01])[.](?|I(?|I{1,2}|[VX])?|VI{0,3}|X(?|I{1,3}|IV|V)?))';
my $nameregex='(\b[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\s[AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoÓóPpRrSsŚśTtUuWwYyZzŹźŻż]+\b)';
my @dates = ($file_content =~ /$dateregex/gi);
my @names = ($file_content =~ /$nameregex(?![\s\S]*\b\1\b)/gi); # Unique names
my @names = sort @names;


my %students;	# Hasmap (Student -> (hashmap: date->activity))
my %activity;

# for (@dates){
# 	$activity{$_} = 0;
# }
# $activity{sum}=0;
for(@names){
	my %hash_copy = %{$activity};
	for (@dates){
		$hash_copy{$_} = 0;
	}
	$hash_copy{sum}= 0;
	$students{$_} = \%hash_copy;
}

# Creating empty activity hashmaps for each student
while (my $line = <$file1>) {
	if ($line =~ /$dateregex/g){	# Reading date
		$cur_date = $1;
	}
	if ($line =~ /$nameregex/g){	# Reading name
		$cur_name = $1;

		$count = () = $line =~ /[+]/g; # Counting activity
		$students{$cur_name}->{$cur_date} += $count;
		$students{$cur_name}->{sum}+=$count;
	}
}

# Student by student info stdout
# binmode STDOUT, ":utf8";
# foreach my $student (@names) {
# 	# print "$student: $students{$student}\n";
# 	print "$student:\n";
# 	foreach my $date (@dates) {
# 		print "$date: $students{$student}->{$date}\n";
# 	}
# 	print "sum: $students{$student}->{sum}\n";
# }

print $out "\t<tr>\n";
print $out "\t\t<th style=\"text-align:left\">Imie</th>\n";
print $out "\t\t<th style=\"text-align:left\">Nazwisko</th>\n";
foreach my $date (@dates) {
	print $out "\t\t<th style=\"text-align:left\">$date</th>\n";
}
print $out "\t\t<th style=\"text-align:left\">Suma</td>\n";
print $out "\t</tr>\n";

foreach my $student (@names) {
	print $out "\t<tr>\n";
	my @subStr = split / /, $student;
	my $name = $subStr[1];
	my $surname = $subStr[0];
	print $out "\t\t<td>$name</td>\n";
	print $out "\t\t<td>$surname</td>\n";
	foreach my $date (@dates) {
		print $out "\t\t<td>$students{$student}->{$date}</td>\n";
	}
	print $out "\t\t<td>$students{$student}->{sum}</td>\n";
	print $out "\t</tr>\n";
}

print $out <<FOOTER;
</table>
</html>
FOOTER
close $file1;
close $out;

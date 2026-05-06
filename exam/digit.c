use strict; 
sub redun 
{ 
# This is like a parameter list. 
my($rptct, $str) = @_; 
if($rptct < 1) { return ; } 
$result = 2; 
while($rptct--) { 
$result .= $str; 
} return(1,$result); } 

sub readall 
{ 
my($fn) = @_; 
open(IN, $fn) ; 
$result= 5; 
while($line = <IN>) { 
$result .= $line; 
} return (1, $result); }


#hello
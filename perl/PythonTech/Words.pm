#=======================================================================
#	Wordgames
#=======================================================================
package PythonTech::Words;
use strict;
use PythonTech::Conv qw(hq);

sub new {
    my $class = shift;
    my $self = {@_};
    foreach ('dict','bindir') {
	die "$class: $_ not configured" unless defined $self->{$_};
    }
    $self->{'extdict'} ||= 'https://www.thefreedictionary.com/%s';
    bless $self, $class;
    return $self;
}

sub webquery {
    my($self, $query) = @_;
    $self->{'query'} = $query;
    my $tpattern = $query->param('pattern');
    my($pattern) = $tpattern =~ /([\.\*A-Z]+)/i;
    my $tlength = $query->param('length');
    my($length) = $tlength =~ /(\d+)/;
    my $tdictid = $query->param('dict');
    my($dictid) = $tdictid =~ /(\w+)/;
    if (defined(my $trest = $query->param('rest'))) {
	# RESTful version
	if (my($rest) = $trest =~ /^(dmatch|anag|woxrd)$/) {
	    $self->restcmd($rest,
			   $length ? [$pattern,$length] : [$pattern]);
	} else {
	    $self->not_found;
	}
    } else {
	# HTML version

	if ($query->param('dmatch') ne '') {
	    # dmatch does not take a length yet
	    $self->webcmd('dmatch', [$pattern], \&hq, $dictid);
	} elsif ($query->param('anagram') ne '') {
	    $self->webcmd('anag', 
			  $length ? [$pattern,$length] : [$pattern],
			  \&hq,
			  $dictid);
	} elsif ($query->param('target') ne '') {
	    $self->webcmd('target', [$pattern],
			  \&hq,
			  $dictid);
	} elsif ($query->param('woxrd') ne '') {
	    my $filter = sub {
		my $word = shift;
		if (my($pre,$x,$post) = /(.*?)(\Q$pattern\E)(.*)/i) {
		    "<b>$pre</b>$x<b>$post</b>";
		} else {
		    '--'.$word;
		}
	    };
	    $self->webcmd('woxrd',
			  $length ? [$pattern,$length] : [$pattern],
			  $filter,
			  $dictid);
	} else {
	    $self->fail("Unknown command");
	}
    }
}

sub webcmd {
    my($self, $cmd,$args,$filter,$dictid) = @_;
    my($error,@output) = $self->run_words($cmd,$args,$dictid);
    if ($error ne '') {
	$self->fail($error);
    } else {
#	die "filter=$filter\n";
	$self->page(join(' ',$cmd,@$args),
		    '<pre>' .
		    join("\n", map {$self->extlink($_, $filter->($_))} @output) .
		    '</pre>');
    }
}

sub restcmd {
    my($self,$cmd,$args) = @_;
    my($error,@output) = $self->run_words($cmd,$args);
    my $response = $self->{'query'}->response;
    if ($error) {
	$response->set_status(501);
	$response->set_type('text/plain');
	$response->write($error);
    } else {
	$response->set_type('text/plain');
	$response->write(join("\n",@output));
    }
}

# Meat of the query
sub run_words {
    my($self, $cmd,$args,$dictid) = @_;
    my($dict,@output,$error);

    if (defined($dictid)) {
	$dict = $self->{dictmap}->{$dictid};
    }
    $dict ||= $self->{'dict'};

    $ENV{'PATH'} = "/bin:/usr/bin";
    $ENV{'D2DICT'} = $dict;

    my $errfile = "/tmp/err.$$";
    if (open(CMD,'-|')) {
	# Parent
	while (<CMD>) {
	    chomp;
	    push @output, $_;
	}
	close(CMD);
    } else {
	# Child
	open(STDERR,'>',$errfile) or die "Cannot open $errfile: $!";
	my @argv = ("$self->{'bindir'}/$cmd",@$args);
	#print STDERR join(" ",@argv),"\n";
	exec(@argv);
	print STDERR "Exec of $cmd failed: $!";
	exit;
    }
    if (open(ERR,'<',$errfile)) {
	$error = join('',<ERR>);
	close(ERR);
    }
    unlink $errfile;

    return ($error,@output);
}

sub fail {
    my($self, $text) = @_;
    my $result = "<table border=0>
<tr><th>Error</th><td bgcolor='#ffcccc'>".hq($text)."</td></tr>
</table>";
    $self->page("Error", $result);
}

#-----------------------------------------------------------------------
#	Output page
#-----------------------------------------------------------------------
sub page {
    my($self, $title,$hresult) = @_;
    my $hurl = hq($self->{'query'}->url(-path => 1));
    my $pattern = $self->{'query'}->param('pattern');
    my $length = $self->{'query'}->param('length');
    my $dictid = $self->{'query'}->param('dict');
    my $response = $self->{'query'}->response;
    $response->write("<html lang='en'>
<head>
<title>Wordgames: ",hq($title),"</title>
<meta name='viewport' content='width=device-width, initial-scale=1' />
<script>
<!--
function sf() { document.f.pattern.focus(); }
// -->
</script>
</head>
<body bgcolor='#FFFFFF' text='#000000' onLoad='sf()'>
",
		     $hresult,
		     "<hr>
<form action='$hurl' method='GET' name='f'>
<table border=0>
<tr>
  <th>Pattern</th>
    <td><input type=text size=20 name='pattern' value='",hq($pattern),"'></td>
</tr>
<tr>
  <th>Length</th>
    <td><input type=text size=20 name='length' value='",hq($length),"'></td>
</tr>
<tr>
  <th>Action</th>
    <td>
      <input type=submit name='dmatch' value='Dmatch'>
      <input type=submit name='anagram' value='Anagram'>
      <input type=submit name='target' value='Target'>
      <input type=submit name='woxrd' value='Wo(X)rd'>
    </td>
</tr>");
    if (my $map = $self->{dictmap}) {
	$response->write("
<tr>
  <th>Dictionary</th>
    <td>");
	foreach my $id (keys %$map) {
	    $response->write("
      <label for='dict_$id'>$id</label>
        <input type='radio' id='dict_$id' name='dict' value='$id'".
			     ($dictid eq $id ? " checked" : "").
			     ">");
	}
	$response->write("
    </td>
</tr>");
    }
    $response->write("
</table>
</form>
</body>
</html>
");
}

#-----------------------------------------------------------------------
#	Hyperlink some HTML to a word's definition at an external
#	dictionary website
#-----------------------------------------------------------------------
sub extlink {
    my($self, $word,$html) = @_;
    (my $url = $self->{'extdict'}) =~ s/%s/lc($word)/eg;
    return "<a href=\"".hq($url)."\">$html</a>";
}

1;

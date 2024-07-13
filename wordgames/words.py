#=======================================================================
#       Wordgames
#=======================================================================
import os
import re
import subprocess
import sys

class Words(object):
    def __init__(self, d2dict, bindir):
        self.d2dict = d2dict
        self.bindir = bindir

    def dmatch(self, pattern, length=None):
        return self._run('dmatch', pattern)

    def anag(self, pattern, length=None):
        return self._run('anag', pattern)

    def target(self, pattern, length=None):
        return self._run('target', pattern)

    def _run(self, exe, pattern, length=None):
        # print('d2dict=%s bindir=%s pattern=%r' %
        #       (self.d2dict, self.bindir, pattern))
        cmd = [os.path.join(self.bindir,exe), pattern]
        opts = {'encoding':'ascii'} if sys.version_info >= (3,6) else {}
        p = subprocess.Popen(cmd, env={'D2DICT': self.d2dict},
                             stdout=subprocess.PIPE,
                             **opts)
        for line in p.stdout:
            yield line.rstrip()
        p.wait()

    def webquery(self, query):
        pattern = query['pattern'] # FIXME check [\.\*A-Z]+ /i
        length = query.get('length')
        if length is not None:
            length = int(length)
        if 'rest' in query:
            rest = query['rest']
            if rest in ('dmatch','anag','woxrd'):
                return self.rest_command(rest, pattern, length)
            else:
                raise NotFound
        else:
            # HTML version
            if 'dmatch' in query:
                return self.webcmd('dmatch', pattern, filter=hq)
            elif 'anagram' in query:
                return self.webcmd('anag', pattern, length, filter=hq)
            elif 'target' in query:
                return self.webcmd('target', pattern, filter=hq)
            elif 'woxrd' in query:
                rx = re.compile
                def filt(word):
                    m = rx.match(word)
                    if m:
                        pre, x, post = m.groups()
                        return '%s<b>%s</b>%s' % (pre, x, post)
                    else:
                        return '--'.word

patRX = re.compile(r'[\.A-Z]+', re.IGNORECASE)

def check_pattern(pattern):
    if not pattern:
        raise ValueError('Empty pattern')
    m = patRX.match(pattern)
    if not m:
        mlen = 0
    else:
        mlen = m.end()
    if mlen < len(pattern):
        raise ValueError('Invalid pattern character %r' % pattern[mlen])
    return pattern.upper()

if __name__=='__main__':
    import argparse

    def pattern_arg(p):
        try:
            return check_pattern(p)
        except ValueError as e:
            raise argparse.ArgumentTypeError(str(e))

    ap = argparse.ArgumentParser()
    ap.add_argument('method', choices=('dmatch',),
                    help='Word search method')
    ap.add_argument('pattern', type=pattern_arg,
                    help='Letters or dots')
    args = ap.parse_args()
    words = Words(os.path.expanduser('~/.local/share/moby'),
                  os.path.expanduser('~/wordgames'))
    if args.method == 'dmatch':
        gen = words.dmatch(args.pattern)
    for word in gen:
        print(word)

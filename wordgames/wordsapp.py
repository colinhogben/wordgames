#=======================================================================
#       Flask application
#=======================================================================
import os
import random
from flask import Flask, request, render_template
from words import Words, check_pattern

app = Flask('wordsapp')
app.secret_key = str(random.randint(0, 99999999999999999999))

words_engine = Words(os.environ['D2DICT'],
                     os.environ['WORDSBIN'])
words_link = os.environ.get('WORDSLINK')

@app.route('/')
def words():
    pattern = request.args.get('pattern')
    if pattern is None:
        pattern = ''
        gen = ()
        return render_template('words.html',
                               pattern=pattern, wordgen=gen)
    else:
        pattern = check_pattern(pattern)
        action = request.args['action']
        if action == 'dmatch':
            gen = words_engine.dmatch(pattern)
        elif action == 'anag':
            gen = words_engine.anag(pattern)
        elif action == 'target':
            gen = words_engine.target(pattern)
        else:
            raise ValueError('unknown action %r' % action)
        if words_link:
            results = [(word, words_link.replace('{}',word.lower()))
                       for word in gen]
        else:
            results = [(word, None)  for word in gen]
        return render_template('words-results.html',
                               pattern=pattern, results=results)

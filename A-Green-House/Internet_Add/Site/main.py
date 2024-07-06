from flask import Flask, render_template, request
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
app=Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI']='sqlite:///data.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS']=False
db=SQLAlchemy(app)

class Article(db.Model):
    date = db.Column(db.DateTime, default=datetime.utcnow)
    time = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Integer)
    hum = db.Column(db.Integer)
    hum_gr = db.Column(db.Integer)
    light = db.Column(db.Integer)

    def __repr__(self):
        return '<Artcile %r>' % self.id


@app.route('/')
def index():
    return "Hello, world!!!"


@app.route('/teplic')
def teplic():
    articles=Article.query.order_by(Article.date).all()
    return render_template("index.html", articles=articles)

@app.route('/teplic/del')
def teplic_del():
    articles=Article.query.all()
    return len(articles)
'''
    for i in range(len(articles)):
        article=Article.query.get_or_404(i)
        try:
            db.session.delete(article)
            db.session.commit()
            return "True"
        except:
            return "False"
'''
@app.route('/teplic/<int:temperature>')
def teplic_in(temperature):
    article=Article(temperature=temperature)
    try:
        db.session.add(article)
        db.session.commit()
        return "True"
    except:
        return "False"


if __name__=="__main__":
    app.run(debug=True)

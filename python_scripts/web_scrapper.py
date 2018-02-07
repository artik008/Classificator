#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import requests

from bs4 import BeautifulSoup

headers = {
	'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
	'Accept-Encoding':'gzip, deflate, br',
	'Accept-Language':'ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7',
	'Cache-Control':'max-age=0',
	'Connection':'keep-alive',
	'Cookie':'dev=0; _ym_uid=1510173146117972356; last_visit=1510162351573::1510173151573; _ym_isad=1; __utmt=1; __utma=11515754.2044262992.1510173151.1511099833.1511106678.3; __utmb=11515754.29.10.1511106678; __utmc=11515754; __utmz=11515754.1511099833.2.2.utmcsr=kino.otzyv.ru|utmccn=(referral)|utmcmd=referral|utmcct=/film/%D0%9C%D0%B0%D1%82%D0%B8%D0%BB%D1%8C%D0%B4%D0%B0-2017/; tmr_detect=1%7C1511110083472',
	'Host':'www.otzyv.ru',
	'Upgrade-Insecure-Requests':'1',
	'User-Agent':'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36'
}

def load_user_data_from_page(user, page):
	payloads = { 'id':str(user), 'data':'6', 'p':str(page)}
	url = 'https://www.otzyv.ru/profile_details.php'
	request = requests.get(url, headers = headers, params = payloads)
	request.encoding = 'cp1251'
	return request.text


def contain_movies_data(text):
    soup = BeautifulSoup(text, "lxml")
    film_requests = soup.find_all('div', {'class': 'film-descr'})
    return film_requests

def get_article(text):
	return (text.split('<p>')[-1]).split('<br/')[0]

def zip_with(id_, arts):
	result = []
	for a in arts:
		result += [id_, arts]
	return result

def get_all_articles():
	result = []
	for id_ in range(20000):
		articles = []
		for page in range(0, 5000, 40):
			articles_on_page = contain_movies_data(load_user_data_from_page(id_, page))
			if articles_on_page == []:
				break
			else:
				articles.append(zip_with(id_, articles_on_page))
				print('Not empty page, user -' + str(id_) + '\n')
		result += articles
	return result


with open('articles', 'w') as output_file:
	count = 0
	articles = get_all_articles()
	for rec in articles:
		a = get_article(str(rec[1]))
		if a != '[]':
			output_file.write('user=' + str(rec[0]) + ':id=' + str(count) + '<article_info>\n')
			output_file.write(a)
			output_file.write('\n\t\n\t\n')
		count += 1

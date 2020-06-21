# Copyright Oleg Nedbaylo 2020.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE
# or copy at http://www.boost.org/LICENSE_1_0.txt

from unittest import TestCase, main
import ssc2ce_cpp as m


class TestCoinbaseParser(TestCase):
    def setUp(self):
        self.parser = m.CoinbaseParser()
        
        self.top_prices = {}
        self.top_bid = 0
        self.top_ask = 0
        self.book_setup_count = 0
        self.parser.set_on_book_setup(self.handle_book_setup)
        self.book_update_count = 0
        self.parser.set_on_book_update(self.handle_book_update)

    def handle_book_setup(self, book):
        if book.instrument() in self.top_prices:
            top = self.top_prices[book.instrument()]
            top["bid"] = book.top_bid_price()
            top["ask"] = book.top_ask_price()
        else:
            self.top_prices[book.instrument()] = dict(
                bid=book.top_bid_price(),
                ask=book.top_ask_price()
            )

        self.book_setup_count += 1

    def handle_book_update(self, book):
        top = self.top_prices[book.instrument()]
        top["bid"] = book.top_bid_price()
        top["ask"] = book.top_ask_price()
        self.book_update_count += 1

    def test_snapshot(self):
        book_snapshot = '{"type":"snapshot","product_id":"BTC-EUR","asks":[["8377.45","2899.99000000"],["8377.46","4771.00000000"]],"bids":[["8377.43","3009.98800000"],["8377.42","4771.00000000"]]}'
        self.assertEqual(self.parser.parse(book_snapshot), True)
        
        book = self.parser.get_book("BTC-EUR")
        self.assertEqual(book.top_ask_price(), 8377.45)
        self.assertEqual(book.top_bid_price(), 8377.43)

        top = self.top_prices[book.instrument()]
        self.assertEqual(top["ask"], 8377.45)
        self.assertEqual(top["bid"], 8377.43)

        self.assertEqual(self.book_setup_count, 1)
        self.assertEqual(self.book_update_count, 0)

    def test_update(self):
        book_snapshot = '{"type":"snapshot","product_id":"BTC-EUR",' \
        '"asks":[["8377.45","2899.99000000"],["8377.46","4771.00000000"]],' \
        '"bids":[["8377.43","3009.98800000"],["8377.42","4771.00000000"]]}'
        self.assertEqual(self.parser.parse(book_snapshot), True)
        book = self.parser.get_book("BTC-EUR")
        top = self.top_prices[book.instrument()]

        self.assertEqual(top["ask"], 8377.45)
        self.assertEqual(top["bid"], 8377.43)

        book_update = '{"type":"l2update","product_id":"BTC-EUR","changes":[' \
        '["buy","8377.44","2.04180000"],' \
        '["sell","8377.45","2.0"]],' \
        '"time":"2020-06-18T10:19:58.975558Z"}'
        self.assertEqual(self.parser.parse(book_update), True)


        self.assertEqual(top["ask"], 8377.45)
        self.assertEqual(top["bid"], 8377.44)

        book_update = '{"type":"l2update","product_id":"BTC-EUR","changes":[["buy","8377.44","0"],["sell","8377.44","4.0"]],"time":"2020-06-18T10:19:58.981559Z"}'
        self.assertEqual(self.parser.parse(book_update), True)
        
        self.assertEqual(top["ask"], 8377.44)
        self.assertEqual(top["bid"], 8377.43)

if __name__ == '__main__':
    main()

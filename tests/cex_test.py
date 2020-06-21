# Copyright Oleg Nedbaylo 2020.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE
# or copy at http://www.boost.org/LICENSE_1_0.txt

from unittest import TestCase, main
import ssc2ce_cpp as m


class TestCoinbaseParser(TestCase):
    def setUp(self):
        self.parser = m.CexParser()

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

    def test_book(self):
        book_snapshot = '{"e":"order-book-subscribe","data":{"timestamp":1592738201,"bids":[[8355.0,0.80000000],[8354.9,0.08562700],[8354.6,0.26770000]],"asks":[[8384.3,0.80000000],[8384.4,0.10000000],[8392.4,1.00000000]],"pair":"BTC:EUR","id":346101544,"sell_total":"176.22612139","buy_total":"1330266.42"},"oid":"book-1", "ok":"ok"}'
        self.assertEqual(self.parser.parse(book_snapshot), True)

        book = self.parser.get_book("BTC:EUR")
        self.assertEqual(book.top_ask_price(), 8384.3)
        self.assertEqual(book.top_bid_price(), 8355.0)

        top = self.top_prices[book.instrument()]
        self.assertEqual(top["ask"], 8384.3)
        self.assertEqual(top["bid"], 8355.0)

        self.assertEqual(self.book_setup_count, 1)
        self.assertEqual(self.book_update_count, 0)

        book_update = '{"e":"md_update","data":{"id":346101545,"pair":"BTC:EUR","time":1592738202189,"bids":[[8355.1,0.08562700],[8354.9,0.00000000]],"asks":[]}}'
        self.assertEqual(self.parser.parse(book_update), True)

        self.assertEqual(top["ask"], 8384.3)
        self.assertEqual(top["bid"], 8355.1)

        book_update = '{"e":"md_update","data":{"id":346101546,"pair":"BTC:EUR","time":1592738206257,"bids":[],"asks":[[8334.9,0.26770000],[8412.2,0.00000000]]}}'
        self.assertEqual(self.parser.parse(book_update), True)

        self.assertEqual(top["ask"], 8334.9)
        self.assertEqual(top["bid"], 8355.1)

        self.assertEqual(self.book_setup_count, 1)
        self.assertEqual(self.book_update_count, 2)


if __name__ == '__main__':
    main()

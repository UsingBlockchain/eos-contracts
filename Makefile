
EOS_PLUGINS=--plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_api_plugin
EOS_FLAGS=--contracts-console
EOS_HTTP=--http-server-address=0.0.0.0:8888 --access-control-allow-origin=*

producer:
	@echo ""
	@echo "\t\t--- Now starting eVias EOS block producer ---"
	@echo ""
	docker run --name="server" --rm -p 8888:8888 -i eosio/eos /bin/bash -c "nodeos -e -p eosio ${EOS_PLUGINS} ${EOS_HTTP} ${EOS_FLAGS} -d /mnt/dev/data"
	@echo ""
	@echo "\t\t--- eVias EOS block producer Started ---"

reader:
	@echo ""
	@echo "\t\t--- Now starting eVias EOS toolchain ---"
	@echo ""
	docker run --name="tools" --rm -v /tmp/work:/work --network="container:server" -it eosio/eos-dev /bin/bash

wallet:
	@echo ""
	@echo "\t\t--- Now starting eVias EOS wallet ---"
	@echo ""
	docker run --name="wallet" --network="container:server" --rm -p 5555:5555 -i eosio/eos /bin/bash -c "keosd --http-server-address=0.0.0:5555"

build:
	@sh build.sh

deploy: clean
	@sh deploy.sh /tmp/work

clean:
	@echo ""
	@echo "\t\t--- Now cleaning eVias EOS Smart Contracts ---"
	@echo ""
	rm -rf /tmp/work


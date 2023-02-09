
TARGETS := wp85

export MANGOH_ROOT ?= $(PWD)/../..

.PHONY: all $(TARGETS)
all: $(TARGETS)

$(TARGETS):
	mkapp -v -t $@ \
	    -i . \
	    -i ../../apps/DataRouter \
		relayControl.adef

system.wp85:
	mksys -v -t wp85 \
	    -i $(LEGATO_ROOT)/interfaces \
	    -i $(LEGATO_ROOT)/interfaces/modemServices \
	    -i $(PWD) \
	    -i $(MANGOH_ROOT)/apps/DataRouter \
		relayControl.sdef

clean:
	rm -rf _build_* *.update

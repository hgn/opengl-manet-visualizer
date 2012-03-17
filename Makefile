
prefix=/usr

INSTALL = /usr/bin/install -c -m 0755
INSTALLDATA = /usr/bin/install -c -m 0644

SUBDIRS = src

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@


install:
	test -d $(prefix) || mkdir --parents $(prefix)
	test -d $(prefix)/share || mkdir --parents $(prefix)/share
	test -d $(prefix)/manet-visualizer || mkdir --parents $(prefix)/share/manet-visualizer
	test -d $(prefix)/manet-visualizer/textures || mkdir --parents $(prefix)/share/manet-visualizer/textures
	test -d $(prefix)/manet-visualizer/textures/ground-01 || mkdir --parents $(prefix)/share/manet-visualizer/textures/ground-01
	test -d $(prefix)/manet-visualizer/textures/skydome-01 || mkdir --parents $(prefix)/share/manet-visualizer/textures/skydome-01
	test -d $(prefix)/manet-visualizer/textures/skydome-02 || mkdir --parents $(prefix)/share/manet-visualizer/textures/skydome-02
	test -d $(prefix)/manet-visualizer/textures/skydome-03 || mkdir --parents $(prefix)/share/manet-visualizer/textures/skydome-03


	$(INSTALLDATA) data/textures/ground-01/* $(prefix)/share/manet-visualizer/textures/ground-01/
	$(INSTALLDATA) data/textures/skydome-01/* $(prefix)/share/manet-visualizer/textures/skydome-01/
	$(INSTALLDATA) data/textures/skydome-02/* $(prefix)/share/manet-visualizer/textures/skydome-02/
	$(INSTALLDATA) data/textures/skydome-03/* $(prefix)/share/manet-visualizer/textures/skydome-03/

	test -d $(prefix)/manet-visualizer/traces || mkdir --parents $(prefix)/share/manet-visualizer/traces
	$(INSTALLDATA) data/traces/* $(prefix)/share/manet-visualizer/traces

	test -d $(prefix)/manet-visualizer/3d-models/misc || mkdir --parents $(prefix)/share/manet-visualizer/3d-models/misc
	$(INSTALLDATA) data/3d-models/misc/* $(prefix)/share/manet-visualizer/3d-models/misc

	$(INSTALL) -m 0755 src/manet-visualizer $(prefix)/bin/


uninstall:
	rm -rf $(prefix)/share/manet-visualizer
	rm -rf $(prefix)/bin/manet-visualizer


.PHONY: install uninstall all
.PHONY: subdirs $(SUBDIRS)

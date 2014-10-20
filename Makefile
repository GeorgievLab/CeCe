.PHONY: clean All

All:
	@echo "----------Building project:[ core - Debug ]----------"
	@cd "core" && $(MAKE) -f  "core.mk"
	@echo "----------Building project:[ gui - Debug ]----------"
	@cd "gui" && $(MAKE) -f  "gui.mk"
clean:
	@echo "----------Cleaning project:[ core - Debug ]----------"
	@cd "core" && $(MAKE) -f  "core.mk"  clean
	@echo "----------Cleaning project:[ gui - Debug ]----------"
	@cd "gui" && $(MAKE) -f  "gui.mk" clean

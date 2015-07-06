#tests_src=$(wildcard test-*.c)
#tests_bin=$(patsubst %.c,%,$(tests_src))
#tests=$(patsubst %.c,%.test-result,$(tests_src))

#$(tests_bin): %: %.c
#	@$(CC) -g3 -D_GNU_SOURCE -Dtest_dir=\"fs-test-$*\" $< -o $@
#setup-%: sfuse
#	@mkdir fs-test-$*
#	@./sfuse fs-test-$*
#run-%: 
#	@./$*
#clear-%: 
#	@sudo umount -l fs-test-$*
#	@rm -rf fs-test-$*
#%.test-result: % setup-% run-% clear-%
#	@:
#tests: $(tests_bin) $(tests)
#	@:


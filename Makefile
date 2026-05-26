ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := legendarios

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx

#---------------------------------------------------------------------------------
# SPRITE SOURCES
# Artist-facing PNGs may be horizontal strips. The generated PNGs below are
# normalized to 64x96 frames stacked vertically before gfx2snes sees them.
assets/build/sprites/guy/idle/guy_idle.png: assets/sprites/guy/idle/guy_idle.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 1

assets/build/sprites/guy/walk/guy_walk.png: assets/sprites/guy/walk/guy_walk.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 8

assets/build/sprites/guy/punch/guy_punch.png: assets/sprites/guy/punch/guy_punch.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 4

assets/build/sprites/guy/kick/guy_kick.png: assets/sprites/guy/kick/guy_kick.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 4

assets/build/sprites/guy/damage/guy_damage.png: assets/sprites/guy/damage/guy_damage.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 3

assets/build/sprites/guy/grab/guy_grab.png: assets/sprites/guy/grab/guy_grab.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 3

assets/build/sprites/guy/jump/guy_jump.png: assets/sprites/guy/jump/guy_jump.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 4

assets/build/sprites/guy/jump_kick/guy_jump_kick.png: assets/sprites/guy/jump_kick/guy_jump_kick.png tools/restructure_sprites.py
	python tools/restructure_sprites.py $< $@ 4

#---------------------------------------------------------------------------------
# BACKGROUNDS (mantidos exatamente como estavam)
assets/backgrounds/stage1/stage1_bg1.pic assets/backgrounds/stage1/stage1_bg1.map assets/backgrounds/stage1/stage1_bg1.pal: assets/backgrounds/stage1/stage1_bg1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -y -s 8 -o 16 -u 16 -e 2 -p -m -t bmp -i $<

assets/backgrounds/stage1/stage1_bg2.pic assets/backgrounds/stage1/stage1_bg2.map assets/backgrounds/stage1/stage1_bg2.pal: assets/backgrounds/stage1/stage1_bg2.bmp
	@echo convert bitmap ... $(notdir $<)
	$(GFXCONV) -g -y -s 8 -o 16 -u 16 -e 4 -p -m -t bmp -i $<

assets/backgrounds/stage1/stage1_bg3.pic assets/backgrounds/stage1/stage1_bg3.map assets/backgrounds/stage1/stage1_bg3.pal: assets/backgrounds/stage1/stage1_bg3.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -g -s 8 -o 4 -u 16 -e 0 -p -m -t bmp -i $<

assets/build/sprites/guy/idle/guy_idle.pic assets/build/sprites/guy/idle/guy_idle.pal: assets/build/sprites/guy/idle/guy_idle.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/walk/guy_walk.pic assets/build/sprites/guy/walk/guy_walk.pal: assets/build/sprites/guy/walk/guy_walk.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/punch/guy_punch.pic assets/build/sprites/guy/punch/guy_punch.pal: assets/build/sprites/guy/punch/guy_punch.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/kick/guy_kick.pic assets/build/sprites/guy/kick/guy_kick.pal: assets/build/sprites/guy/kick/guy_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/damage/guy_damage.pic assets/build/sprites/guy/damage/guy_damage.pal: assets/build/sprites/guy/damage/guy_damage.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/grab/guy_grab.pic assets/build/sprites/guy/grab/guy_grab.pal: assets/build/sprites/guy/grab/guy_grab.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/jump/guy_jump.pic assets/build/sprites/guy/jump/guy_jump.pal: assets/build/sprites/guy/jump/guy_jump.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/build/sprites/guy/jump_kick/guy_jump_kick.pic assets/build/sprites/guy/jump_kick/guy_jump_kick.pal: assets/build/sprites/guy/jump_kick/guy_jump_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/hunter/hunter.pic assets/sprites/hunter/hunter.pal: assets/sprites/hunter/hunter.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/may/may.pic assets/sprites/may/may.pal: assets/sprites/may/may.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/cody/cody.pic assets/sprites/cody/cody.pal: assets/sprites/cody/cody.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/andore/andore.pic assets/sprites/andore/andore.pal: assets/sprites/andore/andore.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

bitmaps : \
	assets/backgrounds/stage1/stage1_bg1.pic \
	assets/backgrounds/stage1/stage1_bg1.map \
	assets/backgrounds/stage1/stage1_bg1.pal \
	assets/backgrounds/stage1/stage1_bg2.pic \
	assets/backgrounds/stage1/stage1_bg2.map \
	assets/backgrounds/stage1/stage1_bg2.pal \
	assets/backgrounds/stage1/stage1_bg3.pic \
	assets/backgrounds/stage1/stage1_bg3.map \
	assets/backgrounds/stage1/stage1_bg3.pal \
	assets/build/sprites/guy/idle/guy_idle.pic \
	assets/build/sprites/guy/idle/guy_idle.pal \
	assets/build/sprites/guy/walk/guy_walk.pic \
	assets/build/sprites/guy/walk/guy_walk.pal \
	assets/build/sprites/guy/punch/guy_punch.pic \
	assets/build/sprites/guy/punch/guy_punch.pal \
	assets/build/sprites/guy/kick/guy_kick.pic \
	assets/build/sprites/guy/kick/guy_kick.pal \
	assets/build/sprites/guy/damage/guy_damage.pic \
	assets/build/sprites/guy/damage/guy_damage.pal \
	assets/build/sprites/guy/grab/guy_grab.pic \
	assets/build/sprites/guy/grab/guy_grab.pal \
	assets/build/sprites/guy/jump/guy_jump.pic \
	assets/build/sprites/guy/jump/guy_jump.pal \
	assets/build/sprites/guy/jump_kick/guy_jump_kick.pic \
	assets/build/sprites/guy/jump_kick/guy_jump_kick.pal

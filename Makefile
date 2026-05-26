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
assets/build/sprites/guy/idle/guy_idle.png: assets/sprites/characters/guy/idle/guy_idle.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 1

assets/build/sprites/guy/walk/guy_walk.png: assets/sprites/characters/guy/walk/guy_walk.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 8

assets/build/sprites/guy/punch/guy_punch.png: assets/sprites/characters/guy/punch/guy_punch.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 4

assets/build/sprites/guy/kick/guy_kick.png: assets/sprites/characters/guy/kick/guy-kick.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 4

assets/build/sprites/guy/damage/guy_damage.png: assets/sprites/characters/guy/damage/guy-damage.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 3

assets/build/sprites/guy/grab/guy_grab.png: assets/sprites/characters/guy/grab/guy-grab.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 3

assets/build/sprites/guy/jump/guy_jump.png: assets/sprites/characters/guy/jump/guy-jump.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 4

assets/build/sprites/guy/dash/guy_dash.png: assets/sprites/characters/guy/dash/guy_dash.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 6

#---------------------------------------------------------------------------------
# BACKGROUNDS (mantidos exatamente como estavam)
assets/backgrounds/stage1/stage1_bg1.pic assets/backgrounds/stage1/stage1_bg1.map assets/backgrounds/stage1/stage1_bg1.pal: assets/backgrounds/stage1/stage1_bg1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -y -s 8 -o 16 -u 16 -e 2 -p -m -t bmp -i $<

assets/backgrounds/stage1/stage1_bg2.pic assets/backgrounds/stage1/stage1_bg2.map assets/backgrounds/stage1/stage1_bg2.pal: assets/backgrounds/stage1/stage1_bg2.bmp
	@echo convert bitmap ... $(notdir $<)
	$(GFXCONV) -g -y -s 8 -o 16 -u 16 -e 4 -p -m -t bmp -i $<

# ============================================================
#  HUD conversion (PNG -> indexed BMP -> .pic + .pal + .map)
# ============================================================
assets/hud/font.pic: tools/gen_hud_font.py
	python tools/gen_hud_font.py

assets/hud/hud.pic assets/hud/hud.map assets/hud/hud.pal: assets/hud/hud.png assets/hud/font.pic tools/convert_hud.py tools/merge_font.py
	python tools/convert_hud.py
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -g -a -R -t bmp -i assets/hud/hud.bmp
	python tools/merge_font.py assets/hud/font.pic assets/hud/hud.pic

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

assets/build/sprites/guy/dash/guy_dash.pic assets/build/sprites/guy/dash/guy_dash.pal: assets/build/sprites/guy/dash/guy_dash.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Dash kick / Run Attack (source horizontal strip, 800x96 -> center crop to 64x96 per frame)
assets/build/sprites/guy/dash/guy_dash_kick.png: assets/sprites/characters/guy/dash/guy_dash_kick.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 8

assets/build/sprites/guy/dash/guy_dash_kick.pic assets/build/sprites/guy/dash/guy_dash_kick.pal: assets/build/sprites/guy/dash/guy_dash_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Super move (source horizontal strip)
assets/build/sprites/guy/super/guy_super.png: assets/sprites/characters/guy/super/super.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 8

assets/build/sprites/guy/super/guy_super.pic assets/build/sprites/guy/super/guy_super.pal: assets/build/sprites/guy/super/guy_super.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Super silhouette (source horizontal strip, non-standard size 416x80 -> pad to 64x96 per frame)
assets/build/sprites/guy/super/guy_superpreto.png: assets/sprites/characters/guy/super/superpreto.png tools/normalize_superpreto.py
	python tools/normalize_superpreto.py $< $@ 4

assets/build/sprites/guy/super/guy_superpreto.pic assets/build/sprites/guy/super/guy_superpreto.pal: assets/build/sprites/guy/super/guy_superpreto.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Jump Kick (source horizontal strip)
assets/build/sprites/guy/jump_kick/guy_jump_kick.png: assets/sprites/characters/guy/jump_kick/guy-jump.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 4

assets/build/sprites/guy/jump_kick/guy_jump_kick.pic assets/build/sprites/guy/jump_kick/guy_jump_kick.pal: assets/build/sprites/guy/jump_kick/guy_jump_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Low Kick (source horizontal strip)
assets/build/sprites/guy/low_kick/guy_low_kick.png: assets/sprites/characters/guy/low_kick/guy_low_kick.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 3

assets/build/sprites/guy/low_kick/guy_low_kick.pic assets/build/sprites/guy/low_kick/guy_low_kick.pal: assets/build/sprites/guy/low_kick/guy_low_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Elbow Drop (source already vertical 64x96, copy to build then convert)
assets/build/sprites/guy/elbow_drop/guy_elbow_drop.png: assets/sprites/characters/guy/elbow_drop/guy_elbow_drop.png
	mkdir -p $(dir $@)
	cp $< $@

assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pic assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pal: assets/build/sprites/guy/elbow_drop/guy_elbow_drop.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

# Senpuukyaku (source horizontal strip)
assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.png: assets/sprites/characters/guy/senpuukyaku/guy_senpuukyaku.png tools/normalize_superpreto.py
	mkdir -p $(dir $@)
	python tools/normalize_superpreto.py $< $@ 8

assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pic assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pal: assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/enemies/hunter/hunter.pic assets/sprites/enemies/hunter/hunter.pal: assets/sprites/enemies/hunter/hunter.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/enemies/may/may.pic assets/sprites/enemies/may/may.pal: assets/sprites/enemies/may/may.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/sprites/enemies/andore/andore.pic assets/sprites/enemies/andore/andore.pal: assets/sprites/enemies/andore/andore.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

bitmaps : \
	assets/backgrounds/stage1/stage1_bg1.pic \
	assets/backgrounds/stage1/stage1_bg1.map \
	assets/backgrounds/stage1/stage1_bg1.pal \
	assets/backgrounds/stage1/stage1_bg2.pic \
	assets/backgrounds/stage1/stage1_bg2.map \
	assets/backgrounds/stage1/stage1_bg2.pal \
	assets/hud/hud.pic \
	assets/hud/hud.map \
	assets/hud/hud.pal \
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
	assets/build/sprites/guy/dash/guy_dash.pic \
	assets/build/sprites/guy/dash/guy_dash.pal \
	assets/build/sprites/guy/dash/guy_dash_kick.pic \
	assets/build/sprites/guy/dash/guy_dash_kick.pal \
	assets/build/sprites/guy/super/guy_super.pic \
	assets/build/sprites/guy/super/guy_super.pal \
	assets/build/sprites/guy/super/guy_superpreto.pic \
	assets/build/sprites/guy/super/guy_superpreto.pal \
	assets/build/sprites/guy/jump_kick/guy_jump_kick.pic \
	assets/build/sprites/guy/jump_kick/guy_jump_kick.pal \
	assets/build/sprites/guy/low_kick/guy_low_kick.pic \
	assets/build/sprites/guy/low_kick/guy_low_kick.pal \
	assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pic \
	assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pal \
	assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pic \
	assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pal

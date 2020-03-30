package com.coldproject.DwarvenTreasure.Entity;


import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import com.coldproject.DwarvenTreasure.Map.Map;
import com.coldproject.DwarvenTreasure.Management.Graphics;

public class Graal extends Entity
{
	BufferedImage[] sprites;

	public Graal(Map map)
	{
		super(map);

		sprites = Graphics.GRAAL[0];
		animation.setFrames(sprites);
		animation.setDelay(10);
	}

	public void update()
	{
		animation.update();
	}

	public void draw(Graphics2D g)
	{
		super.draw(g);
	}
}
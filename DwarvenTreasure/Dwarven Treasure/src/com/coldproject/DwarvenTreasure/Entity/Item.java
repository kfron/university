package com.coldproject.DwarvenTreasure.Entity;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import com.coldproject.DwarvenTreasure.Map.Map;
import com.coldproject.DwarvenTreasure.Management.Graphics;

public class Item extends Entity
{
	private BufferedImage sprite;
	private boolean collected;

	public Item(Map map)
	{
		super(map);

		collected = false;
		sprite = Graphics.ITEM[1][0];
	}

	public void collected(Player p)
	{
		collected = true;
		p.gotPickaxe();
	}

	public boolean isCollected()
	{
		return collected;
	}

	public void draw(Graphics2D g)
	{
		setMapPosition();
		g.drawImage(sprite, x + xmap - 16 / 2, y + ymap - 16 / 2, null);
	}
}
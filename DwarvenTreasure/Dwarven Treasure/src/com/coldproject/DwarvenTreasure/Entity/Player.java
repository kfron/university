package com.coldproject.DwarvenTreasure.Entity;

import java.awt.image.BufferedImage;
import java.awt.Graphics2D;

import com.coldproject.DwarvenTreasure.Map.Map;
import com.coldproject.DwarvenTreasure.Management.Graphics;


public class Player extends Entity
{

	private BufferedImage[] downSprites;
	private BufferedImage[] leftSprites;
	private BufferedImage[] rightSprites;
	private BufferedImage[] upSprites;

	private int goldAmmount;
	private int totalGold;
	private boolean hasPickaxe;

	private final int DOWN = 0;
	private final int LEFT = 1;
	private final int RIGHT = 2;
	private final int UP = 3;

	private long ticks;

	public Player(Map map)
	{
		super(map);

		moveSpeed = 2;

		goldAmmount = 0;

		downSprites = Graphics.PLAYER[0];
		leftSprites = Graphics.PLAYER[1];
		rightSprites = Graphics.PLAYER[2];
		upSprites = Graphics.PLAYER[3];

		animation.setFrames(downSprites);
		animation.setDelay(10);

	}

	public void setAnimation(int i, BufferedImage[] bi, int d)
	{
		currAnimation = i;
		animation.setFrames(bi);
		animation.setDelay(d);
	}

	public void collectedGold()
	{
		goldAmmount++;
	}

	public void collectedGraal()
	{
		goldAmmount = totalGold;
	}

	public int goldAmmount()
	{
		return goldAmmount;
	}

	public int totalGold()
	{
		return totalGold;
	}

	public void settotalGold(int i)
	{
		totalGold = i;
	}

	public void gotPickaxe()
	{
		hasPickaxe = true;
	}

	public boolean hasPickaxe()
	{
		return hasPickaxe;
	}
	public void setDown() {
		super.setDown();
	}
	public void setLeft() {
		super.setLeft();
	}
	public void setRight() {
		super.setRight();
	}
	public void setUp() {
		super.setUp();
	}

	public void setAction() {
		if(hasPickaxe) {
			if(currAnimation == UP && map.getIndex(rowTile - 1, colTile) == 3) {
				map.setTile(rowTile - 1, colTile, 1);
			}
			if(currAnimation == DOWN && map.getIndex(rowTile + 1, colTile) == 3) {
				map.setTile(rowTile + 1, colTile, 1);
			}
			if(currAnimation == LEFT && map.getIndex(rowTile, colTile - 1) == 3) {
				map.setTile(rowTile, colTile - 1, 1);
			}
			if(currAnimation == RIGHT && map.getIndex(rowTile, colTile + 1) == 3) {
				map.setTile(rowTile, colTile + 1, 1);
			}
		}
	}

	public void update()
	{
		ticks++;

		if(down && currAnimation != DOWN)
		{
			setAnimation(DOWN, downSprites, 10);
		}
		else if(left && currAnimation != LEFT)
		{
			setAnimation(LEFT, leftSprites, 10);
		}
		else if(right && currAnimation != RIGHT)
		{
			setAnimation(RIGHT, rightSprites, 10);
		}
		else if(up && currAnimation != UP)
		{
			setAnimation(UP, upSprites, 10);
		}

		super.update();
	}

	public void draw(Graphics2D g)
	{
		super.draw(g);
	}
}

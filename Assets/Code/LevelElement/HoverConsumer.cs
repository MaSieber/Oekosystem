using UnityEngine;
using System.Collections;

public class HoverConsumer : MonoBehaviour 
{
    enum Events
    {
        None = 0,
        EnergyConsume = 1,
    }
    private Events eventType;
    private HoverPlatform hoverPlatform;

	// Use this for initialization
	void Start () 
    {
        hoverPlatform = transform.FindChild("HoverPlatform").gameObject.GetComponent<HoverPlatform>();
	}
	
	// Update is called once per frame
	void Update () 
    {
	
	}
    void OnTriggerEnter2D(Collider2D collider)
    {
        if (collider.gameObject.name == "eventTrigger")
        {
            this.gameObject.GetComponent<BoxCollider2D>().enabled = false;
            eventType = Events.EnergyConsume;
            hoverPlatform.bIsActive = true;
            transform.Find("EnergyButton").gameObject.GetComponent<SpriteRenderer>().color = new Color(0.0f,(237.0f/255.0f),1.0f);
            collider.gameObject.transform.parent.SendMessage("msg_energyConsume", 25);
        }
    }
    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player")
        {
            this.gameObject.GetComponent<BoxCollider2D>().enabled = false;
            eventType = Events.EnergyConsume;
            hoverPlatform.bIsActive = true;
            collision.gameObject.SendMessage("msg_energyConsume", 25);
        }
    }
}

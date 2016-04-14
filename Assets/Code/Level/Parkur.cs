using UnityEngine;
using System.Collections;

public class Parkur : MonoBehaviour {

    //Inspector Settings
    public float fadeOutModifier = 1.0f;
    enum Events
    {
        None = 0,
        EnergySuckOut = 1
    }
    private Events eventType;

	// Use this for initialization
	void Start () {
        eventType = Events.None;
    }
	
	// Update is called once per frame
	void Update ()
    {
        float deltaTime = Time.deltaTime;

        switch (eventType)
        {
            case Events.EnergySuckOut:
                Color col = this.GetComponentInParent<SpriteRenderer>().color;
                col.a -= deltaTime * fadeOutModifier;

                if (col.a <= 0.01f)
                {
                    col.a = 0.0f;
                    eventType = Events.None;
                    this.gameObject.transform.parent.gameObject.GetComponent<BoxCollider2D>().enabled = false;
                    
                }
                    
                this.gameObject.GetComponentInParent<SpriteRenderer>().color = col;

                break;
        }
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            this.gameObject.GetComponent<BoxCollider2D>().enabled = false;
            eventType = Events.EnergySuckOut;
        }
    }

    void msg_EnergySuckOut()
    {
        this.gameObject.GetComponent<BoxCollider2D>().enabled = false;
        eventType = Events.EnergySuckOut;
    }


}
